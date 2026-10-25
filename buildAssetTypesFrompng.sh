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
set -euxo pipefail

cd assets

inkscape Wide.png --export-text-to-path --export-filename=Wide.svg
chafa --symbols ascii -s 100x Wide.png > WideASCII100.txt

inkscape 1_1.png --export-text-to-path --export-filename=1_1.png
chafa --symbols ascii -s 50x 1_1.png > 1_1ASCII50.txt 

cd ..