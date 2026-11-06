::   algosee; A algorithm visulizer
::   Copyright (C) 2026  MrHunor, siryanni (as equals)
::   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano Zapata.
::
::   This program is free software: you can redistribute it and/or modify
::   it under the terms of the GNU General Public License as published by
::   the Free Software Foundation, either version 3 of the License, or
::   (at your option) any later version.
::
::   This program is distributed in the hope that it will be useful,
::   but WITHOUT ANY WARRANTY; without even the implied warranty of
::   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
::   GNU General Public License for more details.
::
::   You should have received a copy of the GNU General Public License
::   along with this program.(root/LICENSE)  If not, see <https://www.gnu.org/licenses/>.
@echo on
setlocal EnableExtensions

conan install . -of build -pr:h default -pr:b default -s build_type=Debug --build=missing || exit /b 1
cmake --preset conan-default || exit /b 1
cmake --build build --parallel || exit /b 1
