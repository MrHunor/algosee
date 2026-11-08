#!/usr/bin/env bash
 #   algosee; A algorithm visulizer
 #   Copyright (C) 2026  MrHunor, siryanni, vilmos45 (as equals)
 #   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano Zapata.
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
 #
 # This is the same as in the gpushWTest.sh, but without the git part
set -euxo pipefail

echo "Testing backend using testclient"
./rebuildLinuxDebug.sh
cd build
./algosee -vvvv &
algosee_pid=$!
trap 'kill "$algosee_pid" 2>/dev/null || true' EXIT
sleep 1s
./algosee_testclient -vvvv -s -p -i
kill "$algosee_pid"
trap - EXIT
cd ..