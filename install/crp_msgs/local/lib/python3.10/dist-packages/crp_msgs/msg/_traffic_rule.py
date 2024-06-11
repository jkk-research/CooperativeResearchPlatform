# generated from rosidl_generator_py/resource/_idl.py.em
# with input from crp_msgs:msg/TrafficRule.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_TrafficRule(type):
    """Metaclass of message 'TrafficRule'."""

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
                'crp_msgs.msg.TrafficRule')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__traffic_rule
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__traffic_rule
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__traffic_rule
            cls._TYPE_SUPPORT = module.type_support_msg__msg__traffic_rule
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__traffic_rule

            from geometry_msgs.msg import PoseWithCovariance
            if PoseWithCovariance.__class__._TYPE_SUPPORT is None:
                PoseWithCovariance.__class__.__import_type_support__()

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


class TrafficRule(metaclass=Metaclass_TrafficRule):
    """Message class 'TrafficRule'."""

    __slots__ = [
        '_header',
        '_stop_pose',
        '_lane_edge_type_left',
        '_lane_edge_type_right',
        '_maximum_speed',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'stop_pose': 'geometry_msgs/PoseWithCovariance',
        'lane_edge_type_left': 'uint8',
        'lane_edge_type_right': 'uint8',
        'maximum_speed': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['geometry_msgs', 'msg'], 'PoseWithCovariance'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        from geometry_msgs.msg import PoseWithCovariance
        self.stop_pose = kwargs.get('stop_pose', PoseWithCovariance())
        self.lane_edge_type_left = kwargs.get('lane_edge_type_left', int())
        self.lane_edge_type_right = kwargs.get('lane_edge_type_right', int())
        self.maximum_speed = kwargs.get('maximum_speed', float())

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
        if self.stop_pose != other.stop_pose:
            return False
        if self.lane_edge_type_left != other.lane_edge_type_left:
            return False
        if self.lane_edge_type_right != other.lane_edge_type_right:
            return False
        if self.maximum_speed != other.maximum_speed:
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
    def stop_pose(self):
        """Message field 'stop_pose'."""
        return self._stop_pose

    @stop_pose.setter
    def stop_pose(self, value):
        if __debug__:
            from geometry_msgs.msg import PoseWithCovariance
            assert \
                isinstance(value, PoseWithCovariance), \
                "The 'stop_pose' field must be a sub message of type 'PoseWithCovariance'"
        self._stop_pose = value

    @builtins.property
    def lane_edge_type_left(self):
        """Message field 'lane_edge_type_left'."""
        return self._lane_edge_type_left

    @lane_edge_type_left.setter
    def lane_edge_type_left(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'lane_edge_type_left' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'lane_edge_type_left' field must be an unsigned integer in [0, 255]"
        self._lane_edge_type_left = value

    @builtins.property
    def lane_edge_type_right(self):
        """Message field 'lane_edge_type_right'."""
        return self._lane_edge_type_right

    @lane_edge_type_right.setter
    def lane_edge_type_right(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'lane_edge_type_right' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'lane_edge_type_right' field must be an unsigned integer in [0, 255]"
        self._lane_edge_type_right = value

    @builtins.property
    def maximum_speed(self):
        """Message field 'maximum_speed'."""
        return self._maximum_speed

    @maximum_speed.setter
    def maximum_speed(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'maximum_speed' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'maximum_speed' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._maximum_speed = value
