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
#!/bin/bash
#This script is here, cuz I am lazy af :D
if [ -z "$1" ]; then
    echo "Fehler: Bitte gib eine Commit-Nachricht an."
    echo "Nutzung: ./gpush.sh \"deine nachricht\""
    exit 1
fi

git add . && git commit -m "$1" && git pull --rebase && git push