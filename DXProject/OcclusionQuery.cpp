#include "OcclusionQuery.h"



OcclusionQuery::OcclusionQuery(Chunk * chunk, ID3D11Query * query) : chunk_(chunk), query_(query) {

}


OcclusionQuery::~OcclusionQuery() {
}

bool OcclusionQuery::operator==(const OcclusionQuery & other) {
	return query_ == other.query_ && chunk_ == other.chunk_;
}

bool OcclusionQuery::getDataIfAvailable(ID3D11DeviceContext * device_context, UINT64 * out_data) {
	UINT64 query_data;

	bool available = S_OK == device_context->GetData(query_, &query_data, sizeof(UINT64), 0);
	if (available) {
		*out_data = query_data;
	}
	return available;
}

UINT64 OcclusionQuery::getData(ID3D11DeviceContext * device_context) {
	UINT64 query_data;

	while (S_OK != device_context->GetData(query_, &query_data, sizeof(UINT64), 0)) {

	}
	char buffer[100];
	sprintf_s(buffer, "Pixels drawn: %d\n", query_data);
	OutputDebugStringA(buffer);
	return query_data;
}

Chunk * OcclusionQuery::getChunk() {
	return chunk_;
}
