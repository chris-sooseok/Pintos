[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/TrWxYekB)
# Pintos Source

This is the starter Pintos source for CSE 521 Operating Systems at UB. This will be updated for each project.

Applied updates:
- PA0
- PA1

Test commit log:
- Nate: from host machine!
- Nate: from docker container via ssh

# Docker commands:
* docker run --name <new_container_name> --mount type=bind,src=</path/to/host/folder>,dst=</path/to/container/mapping> -it <image_name>:<build>
* docker container list
* docker exec -it <container_name> bash
* docker container stop <container_name>
* docker rm <container_name>

# Github ssh keys:
* [Documentation](https://docs.github.com/authentication/connecting-to-github-with-ssh)
* $ ls -al ~/.ssh # Lists the files in your .ssh directory, if they exist
* h-keygen -t ed25519 -C "your_email@example.com" # Generate a new one if empty
* cat ~/.ssh/id.*.pub # To copy and paste for git settings

