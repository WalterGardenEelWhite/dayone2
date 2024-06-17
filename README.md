# simdayone

This repository is for dayone analysis using docker system. You can easily start dayone analysis no matter what your environmnt.

For easy reset, I recommend you to add alias on your .bashrc as
```
alias dockerclean='docker stop $(docker ps -q); docker rm $(docker ps -a -q); docker ps --all'
```

To run a dockerfile,
```
dockerclean && docker build -t smsimulator . && docker run --env="DISPLAY" --volume="$HOME/.Xautority:/root/.Xautority:rw" -p 2222:22 -d --name smsimulator-container -v /home/hyeji/simdayone:/root/smsimulator/work/simdayone smsimulator sleep infinity && docker ps --all && docker exec -it smsimulator-container /bin/bash
```
