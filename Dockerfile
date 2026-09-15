FROM gcc:15-bookworm

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    python3 \
    python3-pip \
    pipx \
    cmake \
    && rm -rf /var/lib/apt/lists/*

ENV PATH="/root/.local/bin:$PATH"

RUN pipx install conan

WORKDIR /app

COPY . .

RUN conan profile detect --force && \
    conan install . \
        --output-folder=build \
        --build=missing \
        -s build_type=Release

RUN cmake --preset conan-release && \
    cmake --build build --parallel

EXPOSE 10000

CMD ["./build/algosee"]