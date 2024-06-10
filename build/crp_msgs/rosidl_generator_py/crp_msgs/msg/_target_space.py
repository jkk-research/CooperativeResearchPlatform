# generated from rosidl_generator_py/resource/_idl.py.em
# with input from crp_msgs:msg/TargetSpace.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_TargetSpace(type):
    """Metaclass of message 'TargetSpace'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('crp_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'crp_msgs.msg.TargetSpace')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__target_space
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__target_space
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__target_space
            cls._TYPE_SUPPORT = module.type_support_msg__msg__target_space
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__target_space

            from autoware_perception_msgs.msg import PredictedObject
            if PredictedObject.__class__._TYPE_SUPPORT is None:
                PredictedObject.__class__.__import_type_support__()

            from crp_msgs.msg import PathWithTrafficRules
            if PathWithTrafficRules.__class__._TYPE_SUPPORT is None:
                PathWithTrafficRules.__class__.__import_type_support__()

            from geometry_msgs.msg import PoseWithCovariance
            if PoseWithCovariance.__class__._TYPE_SUPPORT is None:
                PoseWithCovariance.__class__.__import_type_support__()

            from nav_msgs.msg import OccupancyGrid
            if OccupancyGrid.__class__._TYPE_SUPPORT is None:
                OccupancyGrid.__class__.__import_type_support__()

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class TargetSpace(metaclass=Metaclass_TargetSpace):
    """Message class 'TargetSpace'."""

    __slots__ = [
        '_header',
        '_stopping_pose',
        '_free_space',
        '_path',
        '_relevant_objects',
        '_relevant_obstacles',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'stopping_pose': 'geometry_msgs/PoseWithCovariance',
        'free_space': 'nav_msgs/OccupancyGrid',
        'path': 'crp_msgs/PathWithTrafficRules',
        'relevant_objects': 'sequence<autoware_perception_msgs/PredictedObject>',
        'relevant_obstacles': 'sequence<autoware_perception_msgs/PredictedObject>',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['geometry_msgs', 'msg'], 'PoseWithCovariance'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['nav_msgs', 'msg'], 'OccupancyGrid'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['crp_msgs', 'msg'], 'PathWithTrafficRules'),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.NamespacedType(['autoware_perception_msgs', 'msg'], 'PredictedObject')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.NamespacedType(['autoware_perception_msgs', 'msg'], 'PredictedObject')),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        from geometry_msgs.msg import PoseWithCovariance
        self.stopping_pose = kwargs.get('stopping_pose', PoseWithCovariance())
        from nav_msgs.msg import OccupancyGrid
        self.free_space = kwargs.get('free_space', OccupancyGrid())
        from crp_msgs.msg import PathWithTrafficRules
        self.path = kwargs.get('path', PathWithTrafficRules())
        self.relevant_objects = kwargs.get('relevant_objects', [])
        self.relevant_obstacles = kwargs.get('relevant_obstacles', [])

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.header != other.header:
            return False
        if self.stopping_pose != other.stopping_pose:
            return False
        if self.free_space != other.free_space:
            return False
        if self.path != other.path:
            return False
        if self.relevant_objects != other.relevant_objects:
            return False
        if self.relevant_obstacles != other.relevant_obstacles:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if __debug__:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def stopping_pose(self):
        """Message field 'stopping_pose'."""
        return self._stopping_pose

    @stopping_pose.setter
    def stopping_pose(self, value):
        if __debug__:
            from geometry_msgs.msg import PoseWithCovariance
            assert \
                isinstance(value, PoseWithCovariance), \
                "The 'stopping_pose' field must be a sub message of type 'PoseWithCovariance'"
        self._stopping_pose = value

    @builtins.property
    def free_space(self):
        """Message field 'free_space'."""
        return self._free_space

    @free_space.setter
    def free_space(self, value):
        if __debug__:
            from nav_msgs.msg import OccupancyGrid
            assert \
                isinstance(value, OccupancyGrid), \
                "The 'free_space' field must be a sub message of type 'OccupancyGrid'"
        self._free_space = value

    @builtins.property
    def path(self):
        """Message field 'path'."""
        return self._path

    @path.setter
    def path(self, value):
        if __debug__:
            from crp_msgs.msg import PathWithTrafficRules
            assert \
                isinstance(value, PathWithTrafficRules), \
                "The 'path' field must be a sub message of type 'PathWithTrafficRules'"
        self._path = value

    @builtins.property
    def relevant_objects(self):
        """Message field 'relevant_objects'."""
        return self._relevant_objects

    @relevant_objects.setter
    def relevant_objects(self, value):
        if __debug__:
            from autoware_perception_msgs.msg import PredictedObject
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, PredictedObject) for v in value) and
                 True), \
                "The 'relevant_objects' field must be a set or sequence and each value of type 'PredictedObject'"
        self._relevant_objects = value

    @builtins.property
    def relevant_obstacles(self):
        """Message field 'relevant_obstacles'."""
        return self._relevant_obstacles

    @relevant_obstacles.setter
    def relevant_obstacles(self, value):
        if __debug__:
            from autoware_perception_msgs.msg import PredictedObject
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, PredictedObject) for v in value) and
                 True), \
                "The 'relevant_obstacles' field must be a set or sequence and each value of type 'PredictedObject'"
        self._relevant_obstacles = value
