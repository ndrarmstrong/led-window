FROM node:14-buster

# Install node packages
RUN npm install -g oclif

# Expose service API and MQTT ports
EXPOSE 4000
EXPOSE 4001

# Start in shell
USER node
WORKDIR /home/node
ENTRYPOINT []
CMD /bin/bash