#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SDL2/SDL.h>
#include <memory>
#include <stdexcept>
#include <map>
#include <string_view>

class TextureManager {
private:
	std::map<std::string_view, std::shared_ptr<SDL_Texture>> textures;
public:
	void load_texture(
		const std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>& ren,
		std::string_view path_to_bmp
	) {
		auto sur = std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)>(
			[&](){
				auto s = SDL_LoadBMP(path_to_bmp.data());
				if (!s) throw std::runtime_error("Failed to load bmp.");
				return s;
			}(),
			[](SDL_Surface* s){ if (s) SDL_FreeSurface(s); }
		);
		auto tex = std::shared_ptr<SDL_Texture>(
			[&](){
				auto t = SDL_CreateTextureFromSurface(ren.get(), sur.get());
				if (!t) throw std::runtime_error("Failed to create texture.");
				return t;
			}(),
			[](SDL_Texture*t ){ if (t) SDL_DestroyTexture(t); }
		);
		textures.insert({path_to_bmp, tex});
	}
	std::shared_ptr<SDL_Texture> get_texture(std::string_view path_to_bmp) {
		auto tex = textures.find(path_to_bmp);
		if (tex == textures.end())
			throw std::runtime_error("Failed to find texture.");
		return tex->second;
	}
};

#endif
