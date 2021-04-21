# Source:https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format
# additional target to perform cppcheck run, requires cppcheck

# get all project files
# HACK this workaround is required to avoid qml files checking ^_^
file(GLOB_RECURSE ALL_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.hpp)

add_custom_target(
			cppcheck
			COMMAND /usr/bin/cppcheck
			--enable=warning,performance,portability,information,missingInclude
			--std=c++17
			--library=qt.cfg
			--template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
			--verbose
			--quiet
			--force
			--suppress=ConfigurationNotChecked
			${ALL_SOURCE_FILES}
)
