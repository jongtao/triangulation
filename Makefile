all: cisect

cisect: circleIsect.c
	gcc -lm circleIsect.c -o cisect

clean:
	rm -rf cisect

.PHONY: clean
