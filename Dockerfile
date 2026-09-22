 #   algosee; A algorithm visulizer
 #   Copyright (C) 2026  MrHunor, siryanni (as equals)
 #
 #   This program is free software: you can redistribute it and/or modify
 #   it under the terms of the GNU General Public License as published by
 #   the Free Software Foundation, either version 3 of the License, or
 #   (at your option) any later version.
 #
 #   This program is distributed in the hope that it will be useful,
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 #   GNU General Public License for more details.
 #
 #   You should have received a copy of the GNU General Public License
 #   along with this program.(root/LICENSE)  If not, see <https://www.gnu.org/licenses/>.

FROM gcc:15-bookworm

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    python3 \
    python3-pip \
    pipx \
    cmake \
    && rm -rf /var/lib/apt/lists/#

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