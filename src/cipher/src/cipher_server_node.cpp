#include "rclcpp/rclcpp.hpp"
#include "cipher_interfaces/msg/cipher_message.hpp"
#include "cipher_interfaces/srv/cipher_answer.hpp"

using std::placeholders::_1;
using std::placeholders::_2;



using cipher_interfaces::msg::CipherMessage;
using cipher_interfaces::srv::CipherAnswer;

class CipherNode : public rclcpp::Node
{


public:
    CipherNode() : Node("cipher_server_node")
    {
        pub_ = this -> create_publisher<CipherMessage>("cipher_topic", 10);

        server_ = this -> create_service<CipherAnswer>(
            "verify_answer", std::bind(&CipherNode::answer_handler, this, _1, _2));

        
        
            timer_ = this -> create_wall_timer(
            std::chrono::seconds(1),
            
            std::bind(&CipherNode::publish_cipher, this));

        original_message_ = "LETPYTHONDOMINATETHEWORLD";
        
        key_ = 3;
        
        encoded_ = encoder(original_message_, key_);
    }





private:
    void publish_cipher()
    {
        CipherMessage msg;
        
        msg.message = encoded_;
        msg.key = key_;

        RCLCPP_INFO(this->get_logger(), "publish %s", encoded_.c_str());
        
        
        pub_->publish(msg);

        timer_->cancel(); 
    }







    void answer_handler(const std::shared_ptr<CipherAnswer::Request> request,
                       std::shared_ptr<CipherAnswer::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "received %s", request->answer.c_str());
        
        response->result = (request->answer == original_message_);
        
        if (response->result)
            RCLCPP_INFO(this->get_logger(), "gooood boiii correct decoding");
        else
            RCLCPP_WARN(this->get_logger(), "nuh uhh wwrong decoding");


        rclcpp::shutdown();
    }

    std::string encoder(const std::string &text, int shift)
    {
        std::string result;

        for (char c : text)
        {if (isalpha(c))
            {
                char base = islower(c) ? 'a' : 'A';
                c = (c - base + shift) % 26 + base;}
            result += c;
        }
        
            return result;
    }

    rclcpp::Publisher<CipherMessage>::SharedPtr pub_;
    rclcpp::Service<CipherAnswer>::SharedPtr server_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::string original_message_;
    std::string encoded_;
    
    int key_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CipherNode>());
    rclcpp::shutdown();
    return 0;
}
