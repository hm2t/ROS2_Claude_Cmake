/**
 * @file publisher.cpp
 * @brief ROS2 发布者节点 - 演示如何使用定时器发布 String 消息
 *
 * 该节点创建一个发布者，每 500ms 发布一条包含计数器的 String 消息
 * 到 "topic" 话题。这是 ROS2 发布订阅模式的基础示例。
 */
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <cstdint>
#include <stdexcept>

using namespace std::chrono_literals;

/**
 * @brief 发布者节点类
 *
 * 继承自 rclcpp::Node，提供 ROS2 节点的基本功能。
 * 定时器每 500ms 触发一次消息发布。
 */
class PublisherNode : public rclcpp::Node
{
public:
  PublisherNode()
  : Node("publisher_node"), count_(0)
  {
    try {
      // 创建 QoS 队列长度为 10 的发布者
      // create_publisher 可能因内存不足或中间件初始化失败而返回空指针
      publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
      if (!publisher_) {
        throw std::runtime_error("Failed to create publisher");
      }
      RCLCPP_INFO(this->get_logger(), "Publisher created successfully");

      // 创建 500ms 定时器，绑定到 timer_callback
      // create_wall_timer 可能因系统资源不足而失败
      timer_ = this->create_wall_timer(
        500ms, std::bind(&PublisherNode::timer_callback, this));
      if (!timer_) {
        throw std::runtime_error("Failed to create timer");
      }
      RCLCPP_INFO(this->get_logger(), "Publisher node started, publishing every 500ms");

    } catch (const std::exception & ex) {
      RCLCPP_ERROR(this->get_logger(), "PublisherNode initialization failed: %s", ex.what());
      throw;
    }
  }

private:
  /**
   * @brief 定时器回调函数
   *
   * 每当定时器触发时调用，发布一条包含计数器的消息。
   * 回调函数中的异常会导致整个节点崩溃，因此需要 try-catch 保护。
   */
  void timer_callback()
  {
    try {
      auto message = std_msgs::msg::String();
      message.data = "Hello from cmake_pubsub at " + std::to_string(count_++);

      // 发布前检查 publisher_ 是否有效且 ROS2 系统正常运行
      if (publisher_ && rclcpp::ok()) {
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      } else {
        RCLCPP_WARN(this->get_logger(), "Cannot publish: publisher not ready or system shutting down");
      }
    } catch (const std::exception & ex) {
      RCLCPP_ERROR(this->get_logger(), "Exception in timer_callback: %s", ex.what());
    }
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  uint32_t count_;
};

/**
 * @brief 主函数
 *
 * 初始化 ROS2 系统，创建并运行发布者节点。
 * 使用 try-catch 确保初始化失败时能有错误提示。
 */
int main(int argc, char * argv[])
{
  try {
    rclcpp::init(argc, argv);
    RCLCPP_INFO(rclcpp::get_logger("publisher"), "Initializing publisher node");

    auto node = std::make_shared<PublisherNode>();
    rclcpp::spin(node);

  } catch (const std::exception & ex) {
    // 捕获未被处理的异常，记录错误并返回非零退出码
    std::cerr << "Unhandled exception: " << ex.what() << std::endl;
    rclcpp::shutdown();
    return 1;
  }

  rclcpp::shutdown();
  return 0;
}
