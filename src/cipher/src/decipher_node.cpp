#include "rclcpp/rclcpp.hpp"
#include "cipher_interfaces/msg/cipher_message.hpp"
#include "cipher_interfaces/srv/cipher_answer.hpp"

using cipher_interfaces::msg::CipherMessage;
using cipher_interfaces::srv::CipherAnswer;
using std::placeholders::_1;

class DecipherNode : public rclcpp::Node
{




public:
    DecipherNode() : Node("decipher_node")
        {
        sub_ = this->create_subscription<CipherMessage>(
            "cipher_topic", 10,
            std::bind(&DecipherNode::msg_callback, this, _1));

        client_ = this->create_client<CipherAnswer>("verify_answer");
}


private:
    void msg_callback(const CipherMessage::SharedPtr msg)
    {
        std::string decoded = decoder(msg->message, msg->key);
        RCLCPP_INFO(this->get_logger(), "decoded %s", decoded.c_str());

        
        
        while (!client_->wait_for_service(std::chrono::seconds(1))){
        RCLCPP_INFO(this->get_logger(), "Waitin...");}

        
        auto request = std::make_shared<CipherAnswer::Request>();
        request->answer = decoded;

        
        auto future = client_->async_send_request(request);

        rclcpp::shutdown();
    }

    std::string decoder(const std::string &text, int shift)
    {
        std::string result;
        for (char c : text)
        {
            if (isalpha(c))
            {char base = islower(c) ? 'a' : 'A';
            
            c = (c - base - shift + 26) % 26 + base;
            }
            
            result += c;
        }   
        return result;
    }

    rclcpp::Subscription<CipherMessage>::SharedPtr sub_;
    rclcpp::Client<CipherAnswer>::SharedPtr client_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DecipherNode>());
    return 0;
}
