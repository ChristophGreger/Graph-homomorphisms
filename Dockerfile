FROM ubuntu:latest

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
    curl \
    nodejs \
    npm \
    && rm -rf /var/lib/apt/lists/*

# Install latest CMake
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

# Clone and build the C++ repository
RUN git clone https://github.com/ChristophGreger/Graph-homomorphisms.git /app
WORKDIR /app
RUN chmod +x ./setup.sh && ./setup.sh
RUN mkdir -p build
WORKDIR /app/build
RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_FLAGS="-stdlib=libc++" -DCMAKE_CXX_COMPILER=clang++ .. \
    && cmake --build .

# Clone web repo
RUN git clone https://github.com/FalcoWolff/Graph-homomorphisms-web.git /webapp

# Setup backend
WORKDIR /webapp/backend
RUN npm install
ENV PROGRAM_PATH="/app/build/Graph_homomorphisms"

# Setup frontend
WORKDIR /webapp/frontend
RUN npm install && npm run build

# Expose ports
EXPOSE 3000 80

# Start backend and frontend concurrently
WORKDIR /webapp
RUN npm install -g concurrently serve

CMD concurrently "node backend/server.js" "serve -s frontend/dist -l 80"
