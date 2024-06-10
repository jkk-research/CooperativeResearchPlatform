// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from crp_msgs:msg/Scenario.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "crp_msgs/msg/detail/scenario__struct.h"
#include "crp_msgs/msg/detail/scenario__functions.h"

#include "rosidl_runtime_c/primitives_sequence.h"
#include "rosidl_runtime_c/primitives_sequence_functions.h"

// Nested array functions includes
#include "autoware_perception_msgs/msg/detail/predicted_object__functions.h"
#include "crp_msgs/msg/detail/path_with_traffic_rules__functions.h"
// end nested array functions include
ROSIDL_GENERATOR_C_IMPORT
bool std_msgs__msg__header__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * std_msgs__msg__header__convert_to_py(void * raw_ros_message);
ROSIDL_GENERATOR_C_IMPORT
bool autoware_perception_msgs__msg__predicted_object__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * autoware_perception_msgs__msg__predicted_object__convert_to_py(void * raw_ros_message);
ROSIDL_GENERATOR_C_IMPORT
bool autoware_perception_msgs__msg__predicted_object__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * autoware_perception_msgs__msg__predicted_object__convert_to_py(void * raw_ros_message);
bool crp_msgs__msg__path_with_traffic_rules__convert_from_py(PyObject * _pymsg, void * _ros_message);
PyObject * crp_msgs__msg__path_with_traffic_rules__convert_to_py(void * raw_ros_message);
ROSIDL_GENERATOR_C_IMPORT
bool nav_msgs__msg__occupancy_grid__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * nav_msgs__msg__occupancy_grid__convert_to_py(void * raw_ros_message);
ROSIDL_GENERATOR_C_IMPORT
bool std_msgs__msg__float32__convert_from_py(PyObject * _pymsg, void * _ros_message);
ROSIDL_GENERATOR_C_IMPORT
PyObject * std_msgs__msg__float32__convert_to_py(void * raw_ros_message);

ROSIDL_GENERATOR_C_EXPORT
bool crp_msgs__msg__scenario__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[32];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("crp_msgs.msg._scenario.Scenario", full_classname_dest, 31) == 0);
  }
  crp_msgs__msg__Scenario * ros_message = _ros_message;
  {  // header
    PyObject * field = PyObject_GetAttrString(_pymsg, "header");
    if (!field) {
      return false;
    }
    if (!std_msgs__msg__header__convert_from_py(field, &ros_message->header)) {
      Py_DECREF(field);
      return false;
    }
    Py_DECREF(field);
  }
  {  // local_moving_objects
    PyObject * field = PyObject_GetAttrString(_pymsg, "local_moving_objects");
    if (!field) {
      return false;
    }
    PyObject * seq_field = PySequence_Fast(field, "expected a sequence in 'local_moving_objects'");
    if (!seq_field) {
      Py_DECREF(field);
      return false;
    }
    Py_ssize_t size = PySequence_Size(field);
    if (-1 == size) {
      Py_DECREF(seq_field);
      Py_DECREF(field);
      return false;
    }
    if (!autoware_perception_msgs__msg__PredictedObject__Sequence__init(&(ros_message->local_moving_objects), size)) {
      PyErr_SetString(PyExc_RuntimeError, "unable to create autoware_perception_msgs__msg__PredictedObject__Sequence ros_message");
      Py_DECREF(seq_field);
      Py_DECREF(field);
      return false;
    }
    autoware_perception_msgs__msg__PredictedObject * dest = ros_message->local_moving_objects.data;
    for (Py_ssize_t i = 0; i < size; ++i) {
      if (!autoware_perception_msgs__msg__predicted_object__convert_from_py(PySequence_Fast_GET_ITEM(seq_field, i), &dest[i])) {
        Py_DECREF(seq_field);
        Py_DECREF(field);
        return false;
      }
    }
    Py_DECREF(seq_field);
    Py_DECREF(field);
  }
  {  // local_obstacles
    PyObject * field = PyObject_GetAttrString(_pymsg, "local_obstacles");
    if (!field) {
      return false;
    }
    PyObject * seq_field = PySequence_Fast(field, "expected a sequence in 'local_obstacles'");
    if (!seq_field) {
      Py_DECREF(field);
      return false;
    }
    Py_ssize_t size = PySequence_Size(field);
    if (-1 == size) {
      Py_DECREF(seq_field);
      Py_DECREF(field);
      return false;
    }
    if (!autoware_perception_msgs__msg__PredictedObject__Sequence__init(&(ros_message->local_obstacles), size)) {
      PyErr_SetString(PyExc_RuntimeError, "unable to create autoware_perception_msgs__msg__PredictedObject__Sequence ros_message");
      Py_DECREF(seq_field);
      Py_DECREF(field);
      return false;
    }
    autoware_perception_msgs__msg__PredictedObject * dest = ros_message->local_obstacles.data;
    for (Py_ssize_t i = 0; i < size; ++i) {
      if (!autoware_perception_msgs__msg__predicted_object__convert_from_py(PySequence_Fast_GET_ITEM(seq_field, i), &dest[i])) {
        Py_DECREF(seq_field);
        Py_DECREF(field);
        return false;
      }
    }
    Py_DECREF(seq_field);
    Py_DECREF(field);
  }
  {  // paths
    PyObject * field = PyObject_GetAttrString(_pymsg, "paths");
    if (!field) {
      return false;
    }
    PyObject * seq_field = PySequence_Fast(field, "expected a sequence in 'paths'");
    if (!seq_field) {
      Py_DECREF(field);
      return false;
    }
    Py_ssize_t size = PySequence_Size(field);
    if (-1 == size) {
      Py_DECREF(seq_field);
      Py_DECREF(field);
      return false;
    }
    if (!crp_msgs__msg__PathWithTrafficRules__Sequence__init(&(ros_message->paths), size)) {
      PyErr_SetString(PyExc_RuntimeError, "unable to create crp_msgs__msg__PathWithTrafficRules__Sequence ros_message");
      Py_DECREF(seq_field);
      Py_DECREF(field);
      return false;
    }
    crp_msgs__msg__PathWithTrafficRules * dest = ros_message->paths.data;
    for (Py_ssize_t i = 0; i < size; ++i) {
      if (!crp_msgs__msg__path_with_traffic_rules__convert_from_py(PySequence_Fast_GET_ITEM(seq_field, i), &dest[i])) {
        Py_DECREF(seq_field);
        Py_DECREF(field);
        return false;
      }
    }
    Py_DECREF(seq_field);
    Py_DECREF(field);
  }
  {  // free_space
    PyObject * field = PyObject_GetAttrString(_pymsg, "free_space");
    if (!field) {
      return false;
    }
    if (!nav_msgs__msg__occupancy_grid__convert_from_py(field, &ros_message->free_space)) {
      Py_DECREF(field);
      return false;
    }
    Py_DECREF(field);
  }
  {  // maximum_speed
    PyObject * field = PyObject_GetAttrString(_pymsg, "maximum_speed");
    if (!field) {
      return false;
    }
    if (!std_msgs__msg__float32__convert_from_py(field, &ros_message->maximum_speed)) {
      Py_DECREF(field);
      return false;
    }
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * crp_msgs__msg__scenario__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of Scenario */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("crp_msgs.msg._scenario");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "Scenario");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  crp_msgs__msg__Scenario * ros_message = (crp_msgs__msg__Scenario *)raw_ros_message;
  {  // header
    PyObject * field = NULL;
    field = std_msgs__msg__header__convert_to_py(&ros_message->header);
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "header", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // local_moving_objects
    PyObject * field = NULL;
    size_t size = ros_message->local_moving_objects.size;
    field = PyList_New(size);
    if (!field) {
      return NULL;
    }
    autoware_perception_msgs__msg__PredictedObject * item;
    for (size_t i = 0; i < size; ++i) {
      item = &(ros_message->local_moving_objects.data[i]);
      PyObject * pyitem = autoware_perception_msgs__msg__predicted_object__convert_to_py(item);
      if (!pyitem) {
        Py_DECREF(field);
        return NULL;
      }
      int rc = PyList_SetItem(field, i, pyitem);
      (void)rc;
      assert(rc == 0);
    }
    assert(PySequence_Check(field));
    {
      int rc = PyObject_SetAttrString(_pymessage, "local_moving_objects", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // local_obstacles
    PyObject * field = NULL;
    size_t size = ros_message->local_obstacles.size;
    field = PyList_New(size);
    if (!field) {
      return NULL;
    }
    autoware_perception_msgs__msg__PredictedObject * item;
    for (size_t i = 0; i < size; ++i) {
      item = &(ros_message->local_obstacles.data[i]);
      PyObject * pyitem = autoware_perception_msgs__msg__predicted_object__convert_to_py(item);
      if (!pyitem) {
        Py_DECREF(field);
        return NULL;
      }
      int rc = PyList_SetItem(field, i, pyitem);
      (void)rc;
      assert(rc == 0);
    }
    assert(PySequence_Check(field));
    {
      int rc = PyObject_SetAttrString(_pymessage, "local_obstacles", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // paths
    PyObject * field = NULL;
    size_t size = ros_message->paths.size;
    field = PyList_New(size);
    if (!field) {
      return NULL;
    }
    crp_msgs__msg__PathWithTrafficRules * item;
    for (size_t i = 0; i < size; ++i) {
      item = &(ros_message->paths.data[i]);
      PyObject * pyitem = crp_msgs__msg__path_with_traffic_rules__convert_to_py(item);
      if (!pyitem) {
        Py_DECREF(field);
        return NULL;
      }
      int rc = PyList_SetItem(field, i, pyitem);
      (void)rc;
      assert(rc == 0);
    }
    assert(PySequence_Check(field));
    {
      int rc = PyObject_SetAttrString(_pymessage, "paths", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // free_space
    PyObject * field = NULL;
    field = nav_msgs__msg__occupancy_grid__convert_to_py(&ros_message->free_space);
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "free_space", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // maximum_speed
    PyObject * field = NULL;
    field = std_msgs__msg__float32__convert_to_py(&ros_message->maximum_speed);
    if (!field) {
      return NULL;
    }
    {
      int rc = PyObject_SetAttrString(_pymessage, "maximum_speed", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
