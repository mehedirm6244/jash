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

#include <jash/config.h>

struct Shell_config shell_config;

void jash_init_config() {
  /*
   * These are the default properties for JASH shell
   * will be overwritten by `.jashrc` if it exists
   * To be implemented later
   */
  shell_config.show_welcome = 1;
  shell_config.show_pwd = 1;
  shell_config.prompt_char = "❯";
}
