/*******************************************************************************
 * Copyright 2013-2018 Aerospike, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#include "client.h"
#include "async.h"
#include "conversions.h"
#include "policy.h"
#include "log.h"

extern "C" {
    #include <aerospike/aerospike.h>
    #include <aerospike/as_config.h>
    #include <aerospike/aerospike_index.h>
}

using namespace v8;

/*******************************************************************************
 *  TYPES
 ******************************************************************************/

/**
 *  IndexRemoveCmd — Data to be used in async calls.
 *
 *  libuv allows us to pass around a pointer to an arbitraty object when
 *  running asynchronous functions. We create a data structure to hold the
 *  data we need during and after async work.
 */
typedef struct IndexRemoveCmd {
    aerospike * as;
    int param_err;
    as_error err;
    as_policy_info* policy;
    as_namespace ns;
    char * index;
    LogInfo * log;
    Nan::Persistent<Function> callback;
} IndexRemoveCmd;


/*******************************************************************************
 *  FUNCTIONS
 ******************************************************************************/

/**
 *  prepare() — Function to prepare IndexRemoveCmd, for use in `execute()` and `respond()`.
 *
 *  This should only keep references to V8 or V8 structures for use in
 *  `respond()`, because it is unsafe for use in `execute()`.
 */
static void* prepare(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    Nan::HandleScope scope;

    AerospikeClient* client = Nan::ObjectWrap::Unwrap<AerospikeClient>(info.This());

    IndexRemoveCmd* cmd = new IndexRemoveCmd();
    cmd->as = client->as;
    cmd->param_err = 0;
    cmd->policy = NULL;
    LogInfo* log = cmd->log = client->log;

    Local<Value> maybe_ns = info[0];
    Local<Value> maybe_index_name = info[1];
    Local<Value> maybe_policy = info[2];
    Local<Value> maybe_callback = info[3];

    if (maybe_callback->IsFunction()) {
        cmd->callback.Reset(maybe_callback.As<Function>());
        as_v8_detail(log, "Node.js Callback Registered");
    } else {
        as_v8_error(log, "No callback to register");
        COPY_ERR_MESSAGE(cmd->err, AEROSPIKE_ERR_PARAM);
        cmd->param_err = 1;
        return cmd;
    }

    if (maybe_ns->IsString()) {
        strcpy(cmd->ns, *String::Utf8Value(maybe_ns->ToString()));
        as_v8_detail(log, "The index creation on namespace %s", cmd->ns);
    } else {
        as_v8_error(log, "namespace should be string");
        COPY_ERR_MESSAGE(cmd->err, AEROSPIKE_ERR_PARAM);
        cmd->param_err = 1;
        return cmd;
    }

    if (maybe_index_name->IsString()) {
        cmd->index = strdup(*String::Utf8Value(maybe_index_name->ToString()));
        as_v8_detail(log, "The index to be created %s", cmd->index);
    } else {
        as_v8_error(log, "index name should be passed as a string");
        COPY_ERR_MESSAGE(cmd->err, AEROSPIKE_ERR_PARAM);
        cmd->param_err = 1;
        return cmd;
    }

    if (maybe_policy->IsObject()) {
        cmd->policy = (as_policy_info*)cf_malloc(sizeof(as_policy_info));
        if(infopolicy_from_jsobject(cmd->policy, maybe_policy->ToObject(), log) != AS_NODE_PARAM_OK) {
            as_v8_error(log, "infopolicy shoule be an object");
            COPY_ERR_MESSAGE(cmd->err, AEROSPIKE_ERR_PARAM);
            cmd->param_err = 1;
            return cmd;
        }
    }

    return cmd;
}

/**
 *  execute() — Function to execute inside the worker-thread.
 *
 *  It is not safe to access V8 or V8 data structures here, so everything
 *  we need for input and output should be in the IndexRemoveCmd structure.
 */
static void execute(uv_work_t * req)
{
    // Fetch the IndexRemoveCmd structure
    IndexRemoveCmd* cmd = reinterpret_cast<IndexRemoveCmd*>(req->data);
    aerospike* as = cmd->as;
    as_error* err = &cmd->err;
    as_policy_info* policy = cmd->policy;
    LogInfo* log = cmd->log;

    // Invoke the blocking call.
    // The error is handled in the calling JS code.
    if (as->cluster == NULL) {
        cmd->param_err = 1;
        COPY_ERR_MESSAGE(cmd->err, AEROSPIKE_ERR_PARAM);
        as_v8_error(log, "Not connected to cluster ");
    }

    if (cmd->param_err == 0) {
        as_v8_debug(log, "Invoking aerospike index remove");
        aerospike_index_remove(as, err, policy, cmd->ns, cmd->index);
    }

}

/**
 *  AfterWork — Function to execute when the Work is complete
 *
 *  This function will be run inside the main event loop so it is safe to use
 *  V8 again. This is where you will convert the results into V8 types, and
 *  call the callback function with those results.
 */
static void respond(uv_work_t * req, int status)
{

    Nan::HandleScope scope;
    // Fetch the IndexRemoveCmd structure
    IndexRemoveCmd* cmd = reinterpret_cast<IndexRemoveCmd*>(req->data);
    as_error* err = &cmd->err;
    LogInfo* log = cmd->log;
    as_v8_debug(log, "SINDEX remove : response is");

    Local<Value> argv[1];
    // Build the arguments array for the callback
    if (cmd->param_err == 0) {
        argv[0] = error_to_jsobject(err, log);
    }
    else {
        err->func = NULL;
        as_v8_debug(log, "Parameter error for sindex remove");
        argv[0] = error_to_jsobject(err, log);
    }

    // Surround the callback in a try/catch for safety
    Nan::TryCatch try_catch;

    Local<Function> cb = Nan::New<Function>(cmd->callback);
    // Execute the callback.
    if ( !cb->IsNull() ) {
        Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, 1, argv);
        as_v8_debug(log, "Invoked sindex remove callback");
    }

    // Process the exception, if any
    if ( try_catch.HasCaught() ) {
        Nan::FatalException(try_catch);
    }

    // Dispose the Persistent handle so the callback
    // function can be garbage-collected
    cmd->callback.Reset();

    // clean up any memory we allocated

    if (cmd->param_err == 0) {
        if (cmd->policy != NULL)
        {
            cf_free(cmd->policy);
        }
        as_v8_debug(log, "Cleaned up all the structures");
    }

    delete cmd;
    delete req;
}

/*******************************************************************************
 *  OPERATION
 ******************************************************************************/

/**
 *  The 'put()' Operation
 */
NAN_METHOD(AerospikeClient::IndexRemove)
{
    async_invoke(info, prepare, execute, respond);
}
