FROM --platform=linux/arm64 ubuntu:latest

# Update and install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    wget \
    openjdk-17-jdk \
    libssl-dev \
    clang \
    libc++-dev \
    libc++abi-dev \
    && rm -rf /var/lib/apt/lists/*

# CMake manuell installieren (neueste Version)
RUN wget https://github.com/Kitware/CMake/releases/download/v3.30.5/cmake-3.30.5-linux-aarch64.sh \
    && chmod +x cmake-3.30.5-linux-aarch64.sh \
    && ./cmake-3.30.5-linux-aarch64.sh --skip-license --prefix=/usr/local \
    && rm cmake-3.30.5-linux-aarch64.sh

# Install Boost 1.87.0
RUN wget https://sourceforge.net/projects/boost/files/boost/1.87.0/boost_1_87_0.tar.gz/download -O boost_1_87_0.tar.gz \
    && tar xf boost_1_87_0.tar.gz \
    && cd boost_1_87_0 \
    && ./bootstrap.sh \
    && ./b2 install \
    && cd .. \
    && rm -rf boost_1_87_0.tar.gz boost_1_87_0

# Clone the git repository (replace <repo_url> with actual repo URL)
RUN git clone https://github.com/ChristophGreger/Graph-homomorphisms.git /app

# Set working directory to the cloned repo
WORKDIR /app

# Execute setup.sh from the cloned repository
RUN chmod +x ./setup.sh && ./setup.sh

# Create and navigate to the build directory
RUN mkdir -p build
WORKDIR /app/build

# Run CMake and build in release mode
RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_FLAGS="-stdlib=libc++" -DCMAKE_CXX_COMPILER=clang++ .. \
    && cmake --build .
