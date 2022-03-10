# Script parameters
param (
	[string]$AS_HOST="bob-cluster-a",
	[string]$AS_USER="generic_client",
	[string]$AS_PWD="generic_client",
	[string]$AS_PORT="3000",
	[string]$AS_NAMESPACE="test"
)

function build_nodejs_client {
	param(
		[string]$NODE_VERSION
	)
#  Remove-Item –LiteralPath ".\\node_modules" -Force -Recurse
#  Remove-Item ".\\package-lock.json"
  nvm install $NODE_VERSION
  nvm use $NODE_VERSION
  npm install --unsafe-perm --build-from-source
  # node ${CWD}/node_modules/.bin/mocha --exit --U ${AS_USER} --P ${AS_PWD} --h ${AS_HOST} --port ${AS_PORT} --namespace ${AS_NAMESPACE}
}

build_nodejs_client v10.20.0
build_nodejs_client v12.22.10
build_nodejs_client v14.19.0
build_nodejs_client v16.14.0

nvm use v16.14.0