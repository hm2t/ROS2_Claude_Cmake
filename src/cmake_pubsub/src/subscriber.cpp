/**
 * @file subscriber.cpp
 * @brief ROS2 订阅者节点 - 演示如何订阅消息
 *
 * 该节点订阅 "topic" 话题，接收并打印所有接收到的 String 消息。
 * 这是 ROS2 发布订阅模式的订阅端基础示例。
 */
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <stdexcept>

/**
 * @brief 订阅者节点类
 *
 * 继承自 rclcpp::Node，使用回调函数模式处理接收到的消息。
 * 当有新消息到达 "topic" 话题时，topic_callback 会被自动调用。
 */
class SubscriberNode : public rclcpp::Node
{
public:
  SubscriberNode()
  : Node("subscriber_node")
  {
    try {
      // 创建订阅者，QoS 队列长度为 10
      // create_subscription 可能因无效的 topic 名称或 QoS 设置失败
      subscription_ = this->create_subscription<std_msgs::msg::String>(
        "topic", 10, std::bind(&SubscriberNode::topic_callback, this, std::placeholders::_1));

      if (!subscription_) {
        throw std::runtime_error("Failed to create subscription");
      }
      RCLCPP_INFO(this->get_logger(), "Subscriber node started, listening on /topic");

    } catch (const std::exception & ex) {
      RCLCPP_ERROR(this->get_logger(), "SubscriberNode initialization failed: %s", ex.what());
      throw;
    }
  }

private:
  /**
   * @brief 话题消息回调函数
   *
   * 每当收到新消息时调用。ROS2 保证 msg 指针有效，
   * 但为防御性编程仍进行 null 检查。
   *
   * @param msg 接收到的消息 SharedPtr，勿直接存储
   */
  void topic_callback(const std_msgs::msg::String::SharedPtr msg)
  {
    try {
      // 防御性编程：检查消息是否为 null
      if (!msg) {
        RCLCPP_WARN(this->get_logger(), "Received null message");
        return;
      }

      RCLCPP_INFO(this->get_logger(), "Received: '%s'", msg->data.c_str());

    } catch (const std::exception & ex) {
      RCLCPP_ERROR(this->get_logger(), "Exception in topic_callback: %s", ex.what());
    }
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

/**
 * @brief 主函数
 *
 * 初始化 ROS2 系统，创建并运行订阅者节点。
 */
int main(int argc, char * argv[])
{
  try {
    rclcpp::init(argc, argv);
    RCLCPP_INFO(rclcpp::get_logger("subscriber"), "Initializing subscriber node");

    auto node = std::make_shared<SubscriberNode>();
    rclcpp::spin(node);

  } catch (const std::exception & ex) {
    std::cerr << "Unhandled exception: " << ex.what() << std::endl;
    rclcpp::shutdown();
    return 1;
  }

  rclcpp::shutdown();
  return 0;
}
