//
// Created by selya on 27.02.2019.
//

#include <a2d/renderer/texture/texture_bind_manager.hpp>

namespace a2d {

std::vector<TextureBindManager::TextureUnit> TextureBindManager::units;
std::map<unsigned int, TextureBindManager::TextureUnit *> TextureBindManager::handle_to_unit;
std::list<TextureBindManager::TextureUnit *> TextureBindManager::units_queue;
int TextureBindManager::active_unit = 0;

TextureBindManager::TextureUnit::TextureUnit(int unit_index) : unit_index(unit_index), bound_texture_handle(0) {}

void TextureBindManager::ActiveUnit(int unit_index) {
    if (active_unit == unit_index) return;
    glActiveTexture(GLenum(GL_TEXTURE0 + unit_index));
    active_unit = unit_index;
}

int TextureBindManager::GetActiveUnit() {
    return active_unit;
}

void TextureBindManager::Initialize(int units_count) {
    if (!units.empty()) return;
    units.reserve((unsigned long)units_count);
    for (int i = 0; i < units_count; ++i) {
        units.emplace_back(i);
        auto unit = &units[units.size() - 1];
        unit->iter_in_queue = units_queue.emplace(units_queue.end(), unit);
    }
}

void TextureBindManager::BindTextureToUnit(Texture *texture, int unit_index) {
    // Active required unit
    ActiveUnit(unit_index);
    // Bind texture to active unit
    BindTextureToCurrentUnit(texture);
}

int TextureBindManager::BindTextureToCurrentUnit(Texture *texture) {
    // Get unit
    auto &unit = units[active_unit];
    auto texture_handle = texture->texture_handle;
    // Maybe this texture already bound to this unit?
    if (unit.bound_texture_handle == texture_handle) return active_unit;
    // Unbind old texture
    if (unit.bound_texture_handle) UnbindTexture(unit.bound_texture_handle);
    UnbindTexture(texture_handle);
    // Add handle to map
    handle_to_unit[texture_handle] = &unit;
    // Update unit internals
    unit.bound_texture_handle = texture_handle;
    units_queue.erase(unit.iter_in_queue);
    unit.iter_in_queue = units_queue.emplace(units_queue.end(), &unit);
    texture->BindToGL();
    return active_unit;
}

int TextureBindManager::BindTexture(Texture *texture) {
    auto texture_handle = texture->texture_handle;
    if (!texture_handle) {
        LOG_TRACE("Trying to bind 0 texture handle");
        return -1;
    }
    // Maybe it is already bound?
    auto iter = handle_to_unit.find(texture_handle);
    if (iter != handle_to_unit.end()) {
        return iter->second->unit_index;
    }
    // Take unit with higher priority
    auto unit_index = units_queue.front()->unit_index;
    // Bind texture to that unit
    BindTextureToUnit(texture, unit_index);
    return unit_index;
}

void TextureBindManager::UnbindTexture(unsigned int texture_handle) {
    auto iter = handle_to_unit.find(texture_handle);
    if (iter != handle_to_unit.end()) {
        auto unit = iter->second;
        // Clear unit internals
        unit->bound_texture_handle = 0;
        handle_to_unit.erase(iter);
        units_queue.erase(unit->iter_in_queue);
        unit->iter_in_queue = units_queue.emplace(units_queue.begin(), unit);
        // No actual unbind will happen here
        // It will just prioritize freed texture unit for next binds
    }
}

}