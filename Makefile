.PHONY: release debug clean run

debug:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=DEBUG ..
	cd build; make -j4

release:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=RELEASE ..
	cd build; make -j4

clean:
	rm -rf build

run:
	./build/engine

%:
	@: