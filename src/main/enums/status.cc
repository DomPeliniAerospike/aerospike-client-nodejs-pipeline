/*******************************************************************************
 * Copyright 2013-2023 Aerospike, Inc.
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

#include <nan.h>
#include <node.h>

extern "C" {
#include <aerospike/as_status.h>
}

using namespace v8;

#define set(__obj, __name, __value)                                            \
	Nan::Set(__obj, Nan::New(__name).ToLocalChecked(), Nan::New(__value))

Local<Object> status()
{
	Nan::EscapableHandleScope scope;
	Local<Object> obj = Nan::New<Object>();
	set(obj, "AEROSPIKE_NO_RESPONSE", AEROSPIKE_NO_RESPONSE);
	set(obj, "AEROSPIKE_MAX_ERROR_RATE", AEROSPIKE_MAX_ERROR_RATE);
	set(obj, "AEROSPIKE_USE_NORMAL_RETRY", AEROSPIKE_USE_NORMAL_RETRY);
	set(obj, "AEROSPIKE_ERR_MAX_RETRIES_EXCEEDED", AEROSPIKE_USE_NORMAL_RETRY);
	set(obj, "AEROSPIKE_ERR_ASYNC_QUEUE_FULL", AEROSPIKE_ERR_ASYNC_QUEUE_FULL);
	set(obj, "AEROSPIKE_ERR_CONNECTION", AEROSPIKE_ERR_CONNECTION);
	set(obj, "AEROSPIKE_ERR_TLS_ERROR", AEROSPIKE_ERR_TLS_ERROR);
	set(obj, "AEROSPIKE_ERR_INVALID_NODE", AEROSPIKE_ERR_INVALID_NODE);
	set(obj, "AEROSPIKE_ERR_NO_MORE_CONNECTIONS",
		AEROSPIKE_ERR_NO_MORE_CONNECTIONS);
	set(obj, "AEROSPIKE_ERR_ASYNC_CONNECTION", AEROSPIKE_ERR_ASYNC_CONNECTION);
	set(obj, "AEROSPIKE_ERR_CLIENT_ABORT", AEROSPIKE_ERR_CLIENT_ABORT);
	set(obj, "AEROSPIKE_ERR_INVALID_HOST", AEROSPIKE_ERR_INVALID_HOST);
	set(obj, "AEROSPIKE_NO_MORE_RECORDS", AEROSPIKE_NO_MORE_RECORDS);
	set(obj, "AEROSPIKE_ERR_PARAM", AEROSPIKE_ERR_PARAM);
	set(obj, "AEROSPIKE_ERR_CLIENT", AEROSPIKE_ERR_CLIENT);
	set(obj, "AEROSPIKE_OK", AEROSPIKE_OK);
	set(obj, "AEROSPIKE_ERR_SERVER", AEROSPIKE_ERR_SERVER);
	set(obj, "AEROSPIKE_ERR_RECORD_NOT_FOUND", AEROSPIKE_ERR_RECORD_NOT_FOUND);
	set(obj, "AEROSPIKE_ERR_RECORD_GENERATION",
		AEROSPIKE_ERR_RECORD_GENERATION);
	set(obj, "AEROSPIKE_ERR_REQUEST_INVALID", AEROSPIKE_ERR_REQUEST_INVALID);
	set(obj, "AEROSPIKE_ERR_RECORD_EXISTS", AEROSPIKE_ERR_RECORD_EXISTS);
	set(obj, "AEROSPIKE_ERR_BIN_EXISTS", AEROSPIKE_ERR_BIN_EXISTS);
	set(obj, "AEROSPIKE_ERR_CLUSTER_CHANGE", AEROSPIKE_ERR_CLUSTER_CHANGE);
	set(obj, "AEROSPIKE_ERR_SERVER_FULL", AEROSPIKE_ERR_SERVER_FULL);
	set(obj, "AEROSPIKE_ERR_TIMEOUT", AEROSPIKE_ERR_TIMEOUT);
	set(obj, "AEROSPIKE_ERR_ALWAYS_FORBIDDEN", AEROSPIKE_ERR_ALWAYS_FORBIDDEN);
	set(obj, "AEROSPIKE_ERR_CLUSTER", AEROSPIKE_ERR_CLUSTER);
	set(obj, "AEROSPIKE_ERR_BIN_INCOMPATIBLE_TYPE",
		AEROSPIKE_ERR_BIN_INCOMPATIBLE_TYPE);
	set(obj, "AEROSPIKE_ERR_RECORD_TOO_BIG", AEROSPIKE_ERR_RECORD_TOO_BIG);
	set(obj, "AEROSPIKE_ERR_RECORD_BUSY", AEROSPIKE_ERR_RECORD_BUSY);
	set(obj, "AEROSPIKE_ERR_SCAN_ABORTED", AEROSPIKE_ERR_SCAN_ABORTED);
	set(obj, "AEROSPIKE_ERR_UNSUPPORTED_FEATURE",
		AEROSPIKE_ERR_UNSUPPORTED_FEATURE);
	set(obj, "AEROSPIKE_ERR_BIN_NOT_FOUND", AEROSPIKE_ERR_BIN_NOT_FOUND);
	set(obj, "AEROSPIKE_ERR_DEVICE_OVERLOAD", AEROSPIKE_ERR_DEVICE_OVERLOAD);
	set(obj, "AEROSPIKE_ERR_RECORD_KEY_MISMATCH",
		AEROSPIKE_ERR_RECORD_KEY_MISMATCH);
	set(obj, "AEROSPIKE_ERR_NAMESPACE_NOT_FOUND",
		AEROSPIKE_ERR_NAMESPACE_NOT_FOUND);
	set(obj, "AEROSPIKE_ERR_BIN_NAME", AEROSPIKE_ERR_BIN_NAME);
	set(obj, "AEROSPIKE_ERR_FAIL_FORBIDDEN", AEROSPIKE_ERR_FAIL_FORBIDDEN);
	set(obj, "AEROSPIKE_ERR_FAIL_ELEMENT_NOT_FOUND",
		AEROSPIKE_ERR_FAIL_ELEMENT_NOT_FOUND);
	set(obj, "AEROSPIKE_ERR_FAIL_ELEMENT_EXISTS",
		AEROSPIKE_ERR_FAIL_ELEMENT_EXISTS);
	set(obj, "AEROSPIKE_ERR_ENTERPRISE_ONLY", AEROSPIKE_ERR_ENTERPRISE_ONLY);
	set(obj, "AEROSPIKE_ERR_OP_NOT_APPLICABLE",
		AEROSPIKE_ERR_OP_NOT_APPLICABLE);
	set(obj, "AEROSPIKE_FILTERED_OUT", AEROSPIKE_FILTERED_OUT);
	set(obj, "AEROSPIKE_LOST_CONFLICT", AEROSPIKE_LOST_CONFLICT);
	set(obj, "AEROSPIKE_QUERY_END", AEROSPIKE_QUERY_END);
	set(obj, "AEROSPIKE_SECURITY_NOT_SUPPORTED",
		AEROSPIKE_SECURITY_NOT_SUPPORTED);
	set(obj, "AEROSPIKE_SECURITY_NOT_ENABLED", AEROSPIKE_SECURITY_NOT_ENABLED);
	set(obj, "AEROSPIKE_SECURITY_SCHEME_NOT_SUPPORTED",
		AEROSPIKE_SECURITY_SCHEME_NOT_SUPPORTED);
	set(obj, "AEROSPIKE_INVALID_COMMAND", AEROSPIKE_INVALID_COMMAND);
	set(obj, "AEROSPIKE_INVALID_FIELD", AEROSPIKE_INVALID_FIELD);
	set(obj, "AEROSPIKE_ILLEGAL_STATE", AEROSPIKE_ILLEGAL_STATE);
	set(obj, "AEROSPIKE_INVALID_USER", AEROSPIKE_INVALID_USER);
	set(obj, "AEROSPIKE_USER_ALREADY_EXISTS", AEROSPIKE_USER_ALREADY_EXISTS);
	set(obj, "AEROSPIKE_INVALID_PASSWORD", AEROSPIKE_INVALID_PASSWORD);
	set(obj, "AEROSPIKE_EXPIRED_PASSWORD", AEROSPIKE_EXPIRED_PASSWORD);
	set(obj, "AEROSPIKE_FORBIDDEN_PASSWORD", AEROSPIKE_FORBIDDEN_PASSWORD);
	set(obj, "AEROSPIKE_INVALID_CREDENTIAL", AEROSPIKE_INVALID_CREDENTIAL);
	set(obj, "AEROSPIKE_INVALID_ROLE", AEROSPIKE_INVALID_ROLE);
	set(obj, "AEROSPIKE_ROLE_ALREADY_EXISTS", AEROSPIKE_ROLE_ALREADY_EXISTS);
	set(obj, "AEROSPIKE_INVALID_PRIVILEGE", AEROSPIKE_INVALID_PRIVILEGE);
	set(obj, "AEROSPIKE_NOT_AUTHENTICATED", AEROSPIKE_NOT_AUTHENTICATED);
	set(obj, "AEROSPIKE_ROLE_VIOLATION", AEROSPIKE_ROLE_VIOLATION);
	set(obj, "AEROSPIKE_ERR_UDF", AEROSPIKE_ERR_UDF);
	set(obj, "AEROSPIKE_ERR_BATCH_DISABLED", AEROSPIKE_ERR_BATCH_DISABLED);
	set(obj, "AEROSPIKE_ERR_BATCH_MAX_REQUESTS_EXCEEDED",
		AEROSPIKE_ERR_BATCH_MAX_REQUESTS_EXCEEDED);
	set(obj, "AEROSPIKE_ERR_BATCH_QUEUES_FULL",
		AEROSPIKE_ERR_BATCH_QUEUES_FULL);
	set(obj, "AEROSPIKE_ERR_GEO_INVALID_GEOJSON",
		AEROSPIKE_ERR_GEO_INVALID_GEOJSON);
	set(obj, "AEROSPIKE_ERR_INDEX_FOUND", AEROSPIKE_ERR_INDEX_FOUND);
	set(obj, "AEROSPIKE_ERR_INDEX_NOT_FOUND", AEROSPIKE_ERR_INDEX_NOT_FOUND);
	set(obj, "AEROSPIKE_ERR_INDEX_OOM", AEROSPIKE_ERR_INDEX_OOM);
	set(obj, "AEROSPIKE_ERR_INDEX_NOT_READABLE",
		AEROSPIKE_ERR_INDEX_NOT_READABLE);
	set(obj, "AEROSPIKE_ERR_INDEX", AEROSPIKE_ERR_INDEX);
	set(obj, "AEROSPIKE_ERR_INDEX_NAME_MAXLEN",
		AEROSPIKE_ERR_INDEX_NAME_MAXLEN);
	set(obj, "AEROSPIKE_ERR_INDEX_MAXCOUNT", AEROSPIKE_ERR_INDEX_MAXCOUNT);
	set(obj, "AEROSPIKE_ERR_QUERY_ABORTED", AEROSPIKE_ERR_QUERY_ABORTED);
	set(obj, "AEROSPIKE_ERR_QUERY_QUEUE_FULL", AEROSPIKE_ERR_QUERY_QUEUE_FULL);
	set(obj, "AEROSPIKE_ERR_QUERY_TIMEOUT", AEROSPIKE_ERR_QUERY_TIMEOUT);
	set(obj, "AEROSPIKE_ERR_QUERY", AEROSPIKE_ERR_QUERY);
	set(obj, "AEROSPIKE_ERR_UDF_NOT_FOUND", AEROSPIKE_ERR_UDF_NOT_FOUND);
	set(obj, "AEROSPIKE_ERR_LUA_FILE_NOT_FOUND",
		AEROSPIKE_ERR_LUA_FILE_NOT_FOUND);

	return scope.Escape(obj);
}