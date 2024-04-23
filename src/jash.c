/*
 * This file is a part of jash
 *
 * jash is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jash is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with jash. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include <jash/jash.h>

void jash_version() {
  printf("JASH version %d.%d.%d\n",
         JASH_VERSION_MAJOR, JASH_VERSION_MINOR, JASH_VERSION_PATCH);
}

void jash_welcome() {
  printf("Welcome to JASH V%d.%d.%d !\n",
         JASH_VERSION_MAJOR, JASH_VERSION_MINOR, JASH_VERSION_PATCH);
  printf("To submit an issue, visit https://github.com/mehedirm6244/jash/issues\n");
  printf("Type `help` for assistance\n\n");
}
