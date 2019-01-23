//
// Created by Alexander on 2019-01-04.
//

#ifndef A2D_IOS_BINDINGHEADER_H
#define A2D_IOS_BINDINGHEADER_H

#ifdef __cplusplus
extern "C" {
#endif

void a2d_initialize();
void a2d_step();
void a2d_uninitialize();
void a2d_resolution_changed(int width, int height, int framebuffer_width, int framebuffer_height);
void a2d_touch_event(int touches_count, int touch_index, int touch_phase, float touch_x, float touch_y);

#ifdef __cplusplus
};
#endif


#endif //A2D_IOS_BINDINGHEADER_H
