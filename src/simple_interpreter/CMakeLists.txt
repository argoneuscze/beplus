target_sources(beplus PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/SimpleInterpreter.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Environment.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Value.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/GarbageCollector.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Heap.cpp"
)
