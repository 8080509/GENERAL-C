
#include "General C Tests.h"
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include "stringLib.h"
#include "compilerLib.h"

uint fibTestRec(uint n) {
	if (n < 2) return n;
	return fibTestRec(n - 1) + fibTestRec(n - 2);
};

typedef struct {
	uint num;
	uint res;
} fibTestStack_frame;

#define LIST_TYPE fibTestStack_frame
#include "generalLinkedListLib.h"
#undef LIST_TYPE

uint fibTestStack(uint n) {

	uint out = 0;
	uint num = 0;
	uint res = 0;

	typedef fibTestStack_frame frame_t;
	typedef fibTestStack_frame_list stack_t;

	frame_t frame = { 0 };
	stack_t stack = { 0 };

	frame_t(*pop) (stack_t *stack) = fibTestStack_frame_listPop;
	int(*push) (stack_t *stack, frame_t frame) = fibTestStack_frame_listPrepend;

	frame.num = n;

	enum {
		newFrame,
		midFrame,
		endFrame,
	} state = newFrame;

	while (1) {
		switch (state) {
		case newFrame:
			if (frame.num < 2) {
				frame.res = frame.num;
				state = endFrame;
				continue;
			};
			num = frame.num;
			if (push(&stack, frame)) goto fail;
			frame.num = num - 1;
			frame.res = 0;
			state = newFrame;
			continue;
		case midFrame:
			num = frame.num;
			if (push(&stack, frame)) goto fail;
			frame.num = num - 2;
			frame.res = 0;
			state = newFrame;
			continue;
		case endFrame:
			if (!stack.node) {
				out = frame.res;
				goto cleanup;
			};
			res = frame.res;
			frame = pop(&stack);
			state = frame.res ? endFrame : midFrame;
			frame.res += res;
			continue;
		default:
			assert(0);
		};
	};

fail:
cleanup:

	fibTestStack_frame_listClear(&stack);

	return out;

};

uint fibTestStack_alt(uint n) {

	uint out = 0;
	uint num = 0;
	uint res = 0;

	typedef fibTestStack_frame frame_t;
	typedef frame_t *stack_t;

	frame_t frame = { 0 };
	stack_t stack = NULL;

	stack = malloc(sizeof(frame_t) * n);
	if (!stack) goto fail;
	uint ind = 0;

	//stack_t stack = { 0 };

	//frame_t(*pop) (stack_t *stack) = fibTestStack_frame_listPop;
	//int(*push) (stack_t *stack, frame_t frame) = fibTestStack_frame_listPrepend;

	frame.num = n;

	enum {
		newFrame,
		midFrame,
		endFrame,
	} state = newFrame;

	while (1) {
		switch (state) {
		case newFrame:
			if (frame.num < 2) {
				frame.res = frame.num;
				state = endFrame;
				continue;
			};
			num = frame.num;
			//if (push(&stack, frame)) goto fail;
			stack[ind++] = frame;
			frame.num = num - 1;
			frame.res = 0;
			state = newFrame;
			continue;
		case midFrame:
			num = frame.num;
			//if (push(&stack, frame)) goto fail;
			stack[ind++] = frame;
			frame.num = num - 2;
			frame.res = 0;
			state = newFrame;
			continue;
		case endFrame:
			if (!ind) {
				out = frame.res;
				goto cleanup;
			};
			res = frame.res;
			//frame = pop(&stack);
			frame = stack[--ind];
			state = frame.res ? endFrame : midFrame;
			frame.res += res;
			continue;
		default:
			assert(0);
		};
	};

fail:
cleanup:

	//fibTestStack_frame_listClear(&stack);
	free(stack);

	return out;

};

void recVsStackFibTest(void) {
	while (1) {

		uint num, recRes, stkRes;
		fscanf(stdin, "%u", &num);

		clock_t a, b, c;
		a = clock();
		recRes = fibTestRec(num);
		b = clock();
		stkRes = fibTestStack_alt(num);
		c = clock();

		fprintf(
			stdout,
			"\n"
			"Recursive:  %u\n"
			"\tTime:  %u\n"
			"\n"
			"Stack:      %u\n"
			"\tTime:  %u\n\n",
			recRes,
			b - a,
			stkRes,
			c - b
		);

	};
};

uint infRecTest(uint num) {
	printf("\n%d", num);
	if (num)
		return infRecTest(num - 1) + 1;
	return num;
};

int main(void) {

	//uint num;
	//do {
	//	scanf("%d", &num);

	//	infRecTest(num);
	//} while (num);

	//clear_fibTestStack_frame_listNodeRecyclingStorage();
	//uint num;
	//scanf("%d", &num);
	//printf("\n|%4d|", num || 5);

	float num[3] = { 0 };
	//scanf("%f,%f,%f", num + 0, num + 1, num + 2);
	char buff[0x100];
	fileTokenIter tokIter;
	characterSpecs cSpec = CSPEC_FROM_LITERALS(",\n", " \t");
	fileTokenIterInit(&tokIter, &cSpec, stdin, buff, 0x100, 0);
	string str;
	do {
		printf("Current:  %.2f, %.2f, %.2f\nInput:  ", num[0], num[1], num[2]);
		enum {
			inpt,
			ctrl,
			exit,
		} state = inpt;
		uintptr_t ind = 0;
		while (1) {
			str = fileTokenIterNext(&tokIter);
			if (state == exit && *str.data != '\n') {
				printf("\nERROR:  EXCESSIVE INPUT!\n");
				goto l0;
			};
			switch (*str.data) {
			case ',': // do nothing (perhaps switch state)
				state = inpt;
				ind++;
				if (ind < 3) state = inpt;
				else state = exit;
				break;
			case '\n':
				goto l0;
			default:
				if (state == ctrl) {
					printf("\nERROR:  UNEXPECTED INPUT!\n");
					goto l0;
				};
				sscanf(str.data, "%f", &num[ind]);
			};
		};
	l0:;
	} while (1);

	return 0;
};
