#include <pthread.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

#include "ngine/core/timer.h"
#include "ngine/render/render.h"
#include "ngine/render/render_target.h"
#include "ngine/render/techs/gl44_tech.h"
#include "ngine/scenegraph/scene.h"
#include "ngine/core/log.h"

struct rndr_back_args {
  struct ngine_back_render* 	rndr_back;
  GLFWwindow* 			win;
};

GLFWwindow* create_glfw_window();

void* scenegraph_handler(void*);
void* render_back_handler(struct rndr_back_args*);

struct ngine_sc_node* cam_node;

int main() {
  pthread_t scenegraph_thr;
  pthread_t render_back_thr;
  
  pthread_create(&scenegraph_thr, 0, scenegraph_handler, 0);
  
  sleep(1);
  
  struct ngine_render_target* rndr_target = ngine_render_target_create(cam_node, 1024, 768);
  ngine_render_target_update(rndr_target);
  
  struct ngine_render_back* rndr_back = ngine_render_back_create(create_glfw_window());
  struct ngine_render_front* rndr_front = ngine_render_front_create();
  
  ngine_render_front_set_backend(rndr_front, rndr_back);
  ngine_render_front_add_target(rndr_front, rndr_target, ngine_create_tech_gl44());
  
  ngine_render_front_start(rndr_front);
  ngine_render_back_start(rndr_back);
  
  while(1) { sleep(60); }
  
  ngine_render_front_delete(rndr_front);
  ngine_render_back_delete(rndr_back);
}

void neditor_create_scene(struct ngine_scene* _scene);

void* scenegraph_handler(void* a) {
  struct ngine_scene* scene = ngine_scene_create("scene 0", 1);
  
  neditor_create_scene(scene);
  
  struct ngine_timer* tim = ngine_timer_create();
  ngine_timer_start(tim);
  
  while(1) {
    float diff = ngine_timer_elapsed(tim);
    ngine_scene_update(scene, diff);
  }
  
  ngine_timer_delete(tim);
  ngine_scene_delete(scene);
}

GLFWwindow* create_glfw_window() {
  if(!glfwInit()) { error("glfw init failed"); }
  
//   glfwWindowHint(GLFW_SAMPLES, 4);
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//   glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  GLFWmonitor* monitor = 0;
  if(0/*_fullscrean*/) {
    monitor = glfwGetPrimaryMonitor();
  }
  GLFWwindow* win = glfwCreateWindow(1024, 768, "neditor", monitor, NULL);
  if(!win) {
    error("cannot create window '%s'", "_win_name");
    return NULL;
  }
  
  return win;
}

#include "ngine/scenegraph/sc_node.h"
#include "ngine/scenegraph/entity.h"
#include "ngine/scenegraph/camera.h"
#include "ngine/render/mesh.h"
#include "ngine/import/obj/obj.h"

void suzy_update(struct ngine_sc_node* _sc_node, float _elapsed) {
  _sc_node->pos.x += 0.2*_elapsed;
}

void neditor_create_scene(struct ngine_scene* _scene) {
  struct ngine_mesh* mesh_suzy = ngine_mesh_import_obj("suzanne.obj");
  
  struct ngine_entity* ent_suzy = ngine_entity_create("suzy", mesh_suzy);
  struct ngine_sc_node* node_suzy = ngine_sc_node_create(_scene, "suzanne", NGINE_SC_OBJ_ENTITY);
  node_suzy->pos.z = -5.5;
  tree_add_child(_scene->root_object, node_suzy);
  node_suzy->attached_obj = ent_suzy;
  node_suzy->listener->on_update = suzy_update;
  
  struct ngine_sc_node* node_cam = ngine_sc_node_create(_scene, "camera", NGINE_SC_OBJ_CAMERA);
  node_cam->attached_obj = ngine_camera_create();
  ngine_camera_nearfar(node_cam->attached_obj, 0.1, 100.0);
  ngine_camera_persp(node_cam->attached_obj, 45.0);
  tree_add_child((struct tree*)_scene->root_object, (struct tree*)node_cam);
  
  cam_node = node_cam;
}
