from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
   return LaunchDescription([
        Node(
            package='cipher_game',
            executable='cipher_server_node',
            name='cipher_server',
        ),
        Node(
            package='cipher_game',
            executable='decipher_node',
            name='decipher',
        )

   ])