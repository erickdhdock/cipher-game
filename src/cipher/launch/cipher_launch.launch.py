from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
   return LaunchDescription([
        Node(
            package='cipher',
            executable='cipher_server_node',
            name='cipher_server',
        ),
        Node(
            package='cipher',
            executable='decipher_node',
            name='decipher',
        )

   ])