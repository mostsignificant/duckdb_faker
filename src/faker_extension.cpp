#define DUCKDB_EXTENSION_MAIN

#include "faker_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void FakerScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Faker "+name.GetString()+" 🐥");;
        });
}

inline void FakerOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Faker " + name.GetString() +
                                                     ", my linked OpenSSL version is " +
                                                     OPENSSL_VERSION_TEXT );;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
    // Register a scalar function
    auto faker_scalar_function = ScalarFunction("faker", {LogicalType::VARCHAR}, LogicalType::VARCHAR, FakerScalarFun);
    ExtensionUtil::RegisterFunction(instance, faker_scalar_function);

    // Register another scalar function
    auto faker_openssl_version_scalar_function = ScalarFunction("faker_openssl_version", {LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, FakerOpenSSLVersionScalarFun);
    ExtensionUtil::RegisterFunction(instance, faker_openssl_version_scalar_function);
}

void FakerExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string FakerExtension::Name() {
	return "faker";
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void faker_init(duckdb::DatabaseInstance &db) {
    duckdb::DuckDB db_wrapper(db);
    db_wrapper.LoadExtension<duckdb::FakerExtension>();
}

DUCKDB_EXTENSION_API const char *faker_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
