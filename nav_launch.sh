
# Pulling Docker Image 
docker pull ghcr.io/hn-on-fire/mrm-erc2022-maintenencestack:latest

# Running Docker Image 
docker run -it --net=host -e ROS_IP -e ROS_MASTER_URI --env-file credentials.env --name mrm_img ghcr.io/blank-wastaken/mrm-erc2022-navstack

