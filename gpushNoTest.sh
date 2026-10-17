#!/usr/bin/env bash
 #   algosee; A algorithm visulizer
 #   Copyright (C) 2026  MrHunor, siryanni (as equals)
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
 # This script is for the true sigmas
set -euxo pipefail
if [ -z "$1" ]; then
    echo "Error: Please specify a commit message."
    echo "Usage: ./gpush.sh \"commit message\""
    exit 1
fi
git add . && git commit -m "$1" && git pull origin development --rebase && git push origin development