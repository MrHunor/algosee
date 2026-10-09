/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano Zapata.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.(root/LICENSE)  If not, see
 * <https://www.gnu.org/licenses/>.
 */

 #ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "../../logger/logger.h"
#include "../../config.h"
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>

#include <stb_image.h>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <random>



image loadImage(const httplib::Request &req, const stateClass &state) {

  // read image from body to file
  if (!req.form.has_file("image"))
    throw excep("Request does not contain a 'image' file.");

  const auto &image = req.form.get_file("image");

  std::ofstream file("originalImage.png", std::ios::binary);
  file.write(image.content.data(), image.content.size());
  file.close();

  // read file to memory
  int width = 0;
  int height = 0;
  int channels = 0;

  colour *data =
      stbi_load("originalImage.png", &width, &height, &channels,
                4); // the 4 FORCES RGBA (might lead to issues later on)

  if (!data)
    throw excep("Failed to read saved image into memory.");

  // copy to vector
  std::vector<colour> colours(data, data + (width * height * 4));

  stbi_image_free(data);


  //copy to pixel vector
  std::vector<pixel> pixels;
  pixels.reserve(colours.size() / 4);
  pixel currentPixel;
  for (int i = 0; i < colours.size(); i = i + 4) {
    currentPixel.R = colours[i + 0];
    currentPixel.G = colours[i + 1];
    currentPixel.B = colours[i + 2];
    currentPixel.A = colours[i + 3];
    pixels.push_back(currentPixel);
  }

  //add index Data
  indexPixel indexPixeldata;
  indexPixeldata.reserve(pixels.size());
  for (int i = 0; i < pixels.size(); i++) {
    indexPixeldata.push_back({i, pixels[i]});
  }

  //add everthing up to a image struct
  struct image retval;
  retval.width = width;
  retval.height = height;
  retval.data = indexPixeldata;


  return retval;
}


void shuffleImage(image& img)
{
std::random_device rd;
std::mt19937 g(rd());

std::shuffle(img.data.begin(),img.data.end(),g);


}