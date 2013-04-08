/* Auto-generated by genmsg_cpp for file /opt/ros/cturtle/stacks/m3_msgs/msg/M3BaseStatus.msg */
#ifndef M3_MSGS_MESSAGE_M3BASESTATUS_H
#define M3_MSGS_MESSAGE_M3BASESTATUS_H
#include <string>
#include <vector>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/message.h"
#include "ros/time.h"


namespace m3_msgs
{
template <class ContainerAllocator>
struct M3BaseStatus_ : public ros::Message
{
  typedef M3BaseStatus_<ContainerAllocator> Type;

  M3BaseStatus_()
  : name()
  , state(0)
  , timestamp(0)
  , rate()
  , version()
  {
  }

  M3BaseStatus_(const ContainerAllocator& _alloc)
  : name(_alloc)
  , state(0)
  , timestamp(0)
  , rate(_alloc)
  , version(_alloc)
  {
  }

  typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _name_type;
  std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  name;

  typedef uint8_t _state_type;
  uint8_t state;

  typedef int64_t _timestamp_type;
  int64_t timestamp;

  typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _rate_type;
  std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  rate;

  typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _version_type;
  std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  version;


private:
  static const char* __s_getDataType_() { return "m3_msgs/M3BaseStatus"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getDataType() { return __s_getDataType_(); }

  ROSCPP_DEPRECATED const std::string __getDataType() const { return __s_getDataType_(); }

private:
  static const char* __s_getMD5Sum_() { return "7d453ab91423b010362aa62a9d8b20ce"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMD5Sum() { return __s_getMD5Sum_(); }

  ROSCPP_DEPRECATED const std::string __getMD5Sum() const { return __s_getMD5Sum_(); }

private:
  static const char* __s_getMessageDefinition_() { return "string name\n\
uint8 state\n\
int64 timestamp\n\
string rate\n\
string version\n\
\n\
\n\
"; }
public:
  ROSCPP_DEPRECATED static const std::string __s_getMessageDefinition() { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED const std::string __getMessageDefinition() const { return __s_getMessageDefinition_(); }

  ROSCPP_DEPRECATED virtual uint8_t *serialize(uint8_t *write_ptr, uint32_t seq) const
  {
    ros::serialization::OStream stream(write_ptr, 1000000000);
    ros::serialization::serialize(stream, name);
    ros::serialization::serialize(stream, state);
    ros::serialization::serialize(stream, timestamp);
    ros::serialization::serialize(stream, rate);
    ros::serialization::serialize(stream, version);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint8_t *deserialize(uint8_t *read_ptr)
  {
    ros::serialization::IStream stream(read_ptr, 1000000000);
    ros::serialization::deserialize(stream, name);
    ros::serialization::deserialize(stream, state);
    ros::serialization::deserialize(stream, timestamp);
    ros::serialization::deserialize(stream, rate);
    ros::serialization::deserialize(stream, version);
    return stream.getData();
  }

  ROSCPP_DEPRECATED virtual uint32_t serializationLength() const
  {
    uint32_t size = 0;
    size += ros::serialization::serializationLength(name);
    size += ros::serialization::serializationLength(state);
    size += ros::serialization::serializationLength(timestamp);
    size += ros::serialization::serializationLength(rate);
    size += ros::serialization::serializationLength(version);
    return size;
  }

  typedef boost::shared_ptr< ::m3_msgs::M3BaseStatus_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::m3_msgs::M3BaseStatus_<ContainerAllocator>  const> ConstPtr;
}; // struct M3BaseStatus
typedef  ::m3_msgs::M3BaseStatus_<std::allocator<void> > M3BaseStatus;

typedef boost::shared_ptr< ::m3_msgs::M3BaseStatus> M3BaseStatusPtr;
typedef boost::shared_ptr< ::m3_msgs::M3BaseStatus const> M3BaseStatusConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::m3_msgs::M3BaseStatus_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::m3_msgs::M3BaseStatus_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace m3_msgs

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator>
struct MD5Sum< ::m3_msgs::M3BaseStatus_<ContainerAllocator> > {
  static const char* value() 
  {
    return "7d453ab91423b010362aa62a9d8b20ce";
  }

  static const char* value(const  ::m3_msgs::M3BaseStatus_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x7d453ab91423b010ULL;
  static const uint64_t static_value2 = 0x362aa62a9d8b20ceULL;
};

template<class ContainerAllocator>
struct DataType< ::m3_msgs::M3BaseStatus_<ContainerAllocator> > {
  static const char* value() 
  {
    return "m3_msgs/M3BaseStatus";
  }

  static const char* value(const  ::m3_msgs::M3BaseStatus_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::m3_msgs::M3BaseStatus_<ContainerAllocator> > {
  static const char* value() 
  {
    return "string name\n\
uint8 state\n\
int64 timestamp\n\
string rate\n\
string version\n\
\n\
\n\
";
  }

  static const char* value(const  ::m3_msgs::M3BaseStatus_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::m3_msgs::M3BaseStatus_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.name);
    stream.next(m.state);
    stream.next(m.timestamp);
    stream.next(m.rate);
    stream.next(m.version);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct M3BaseStatus_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::m3_msgs::M3BaseStatus_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::m3_msgs::M3BaseStatus_<ContainerAllocator> & v) 
  {
    s << indent << "name: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.name);
    s << indent << "state: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.state);
    s << indent << "timestamp: ";
    Printer<int64_t>::stream(s, indent + "  ", v.timestamp);
    s << indent << "rate: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.rate);
    s << indent << "version: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.version);
  }
};


} // namespace message_operations
} // namespace ros

#endif // M3_MSGS_MESSAGE_M3BASESTATUS_H

