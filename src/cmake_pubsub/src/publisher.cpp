/**
 * @file publisher.cpp
 * @brief ROS2 发布者节点 - 演示定时器发布 String 消息
 */
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <cstdint>
#include <stdexcept>

using namespace std::chrono_literals;

static constexpr char TOPIC_NAME[] = "topic";

class PublisherNode : public rclcpp::Node
{
public:
  PublisherNode()
  : Node("publisher_node"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>(TOPIC_NAME, 10);
    timer_ = this->create_wall_timer(500ms, std::bind(&PublisherNode::timer_callback, this));
    RCLCPP_INFO(this->get_logger(), "Publisher node started");
  }

private:
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello from cmake_pubsub at " + std::to_string(count_++);
    publisher_->publish(message);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  uint32_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PublisherNode>());
  rclcpp::shutdown();
  return 0;
}
