#ifndef CNN_H
#define CNN_H
#include"mlp.h"
#ifndef __cplusplus
typedef struct ConvLayer ConvLayer;
typedef struct PoolLayer PoolLayer;
typedef struct CNN CNN;
#endif
//������ Convolution Layer�� �����ϴ� ����ü�ν� �� �ǽ������� Convolution�� stride�� 1 �� �����Ѵ�.
struct ConvLayer {
	float* feature;		//��������� feature. [depth][f_size][f_size] �� ũ�⸦ ������ 3���� ��Ʈ����.
	float* error;			//��������� error.  [depth][f_size][f_size] �� ũ�⸦ ������ 3���� ��Ʈ����.
	float* kernel;			//feature�� ����� ���� kernel. [depth][k_size][k_size] �� ũ�⸦ ������ 3���� ��Ʈ����.
	int f_size;			//feature�� width, height
	int k_size;			//kernel�� width, height
	int depth;
};
struct PoolLayer {
	float* feature;		//Ǯ���� feature. [depth][f_size][f_size] 3���� ��Ʈ����
	float* error;			//Ǯ���� error.
	int depth;
	int f_size;
};
struct CNN {
	int num_of_convolution;		//��������� ����, Ǯ���� ����. �� ���� 2�̸� C1->S1->C1->S2 �� ���¸� ������ ��.
	float* input;						//�Է� �̹���.
	int input_size;						//�Է� �̹����� width,height
	int input_depth;					//�Է� �̹����� ä��
	ConvLayer* conv;					//ConvLayer���� �迭
	PoolLayer* pool;					//PoolLayer���� �迭
	Network network;					//Fully-Connected ���̾�(MLP�� ����)
};
inline CNN CreateCNN(int input_size, int input_depth, int num_of_convolution, int* kernel_depths, int kernel_size) {
	/*
	*	@TODO
	*	1. CNN����ü�� num_of_convolution, input_size, input_depth�� �ʱ�ȭ.
	*	2. conv, pool ���̾ num_of_convolution��ŭ �Ҵ�.
	*	3. Convolution�� stride�� 1�� ����.
	*	4. kernel_depths �� ���� �� ���� depth�� �ʱ�ȭ �� �� �ְ�,
	*	5. kernel_size�� ���� �� ���� feature size���� ��� ����� �� �ֽ�.
	*	6. MLP�� ���� kernel�� [-1,1] �� �ʱ�ȭ�Ѵ�.
	*/
	CNN cnn;
	cnn.num_of_convolution = num_of_convolution;
	cnn.input_size = input_size;
	cnn.input_depth = input_depth;
	cnn.conv = (ConvLayer*)calloc(cnn.num_of_convolution, sizeof(ConvLayer));
	cnn.pool = (PoolLayer*)calloc(cnn.num_of_convolution, sizeof(PoolLayer));
	int prev_size = input_size;
	int prev_depth = input_depth;
	for (int i = 0; i < num_of_convolution; i++) {
		cnn.conv[i].depth = kernel_depths[i] / prev_depth;															//���� ������ feature�� depth�� input kernel depth���� prev depth�� ������ �־�� ��
		cnn.conv[i].k_size = kernel_size;
		cnn.conv[i].kernel = (float*)calloc(cnn.conv[i].k_size * cnn.conv[i].k_size * kernel_depths[i], sizeof(float));


		cnn.conv[i].f_size = prev_size - (kernel_size - 1);
		cnn.conv[i].feature = (float*)calloc(cnn.conv[i].f_size * cnn.conv[i].f_size * cnn.conv[i].depth, sizeof(float));
		cnn.conv[i].error = (float*)calloc(cnn.conv[i].f_size * cnn.conv[i].f_size * cnn.conv[i].depth, sizeof(float));
		for (int j = 0; j < cnn.conv[i].k_size * cnn.conv[i].k_size * kernel_depths[i]; j++) {
			cnn.conv[i].kernel[j] = (float)rand() / RAND_MAX * 2 - 1.0F;
		}
		printf("conv : %dx%dx%d, %dx%dx%d\n", cnn.conv[i].f_size, cnn.conv[i].f_size, cnn.conv[i].depth, cnn.conv[i].k_size, cnn.conv[i].k_size, cnn.conv[i].depth);

		////////////////////////////////////////////////////////////////////////////////
		//
		//
		// PROBLEM 2: ADD YOUR CODE BELOW
		//
		// �־��� �ǽ��� Pooling layer������ 2 x 2 filter Max Pooling�� ����մϴ�.
		// �� ������ ������ �Ʒ��� ��ĭ �ڵ带 �ۼ��ϼ���.
		//
		//
		////////////////////////////////////////////////////////////////////////////////


		cnn.pool[i].depth = cnn.conv[i].depth;/* ADD YOUR CODE HERE */
		cnn.pool[i].f_size = cnn.conv[i].f_size / 2;/* ADD YOUR CODE HERE */
		cnn.pool[i].feature = (float*)calloc(cnn.pool[i].f_size * cnn.pool[i].f_size * cnn.pool[i].depth, sizeof(float));
		cnn.pool[i].error = (float*)calloc(cnn.pool[i].f_size * cnn.pool[i].f_size * cnn.pool[i].depth, sizeof(float));
		prev_size = cnn.pool[i].f_size;
		prev_depth = cnn.conv[i].depth;
		printf("pool : %dx%dx%d\n", cnn.pool[i].f_size, cnn.pool[i].f_size, cnn.pool[i].depth);
	}
	/*
	*	Conv�ڿ� ����� MLP�� ����.
	*/
	int size_of_layers[] = { cnn.pool[num_of_convolution - 1].f_size * cnn.pool[num_of_convolution - 1].f_size * cnn.pool[num_of_convolution - 1].depth,100,10 };
	if (num_of_convolution == 0) {
		size_of_layers[0] = input_size * input_size * input_depth;
	}
	for (int i = 0; i < sizeof(size_of_layers) / sizeof(int); i++) {
		printf("fc : %d\n", size_of_layers[i]);
	}
	cnn.network = CreateNetwork(size_of_layers, 3);
	return cnn;
}
inline void Convolution3D(float* input3D, int input_size, int input_depth, ConvLayer* conv) {
	/*
	*	@TODO
	*	3���� �ټ��� input3D�� 4���� �ټ��� ConvLayer::kernel�� �̿��Ͽ�
	*	3���� �ټ� ConvLayer::feature�� ����.
	*	@hint
	*	����ȭ�� 3,4���� �ټ��� ���� ���� ����� �����Ƿ�, 2�����ټ��� �����Ͽ� ���.
	*	Convolution�� �⺻ ������ ����Ͽ� �����ϸ� 6�� for���� ������ ��.
	*	@warning!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*	�� �κп��� �Ǽ��Ͽ���, ���ʿ� FC�� �з��� ���ֱ� ������  ��������� �� �����ϴ� ��ó�� ����.
	*	�ùٸ��� ������ CNN�� MNIST validation �����Ϳ��� 95% �̻��� ���;� ��.
	*/
	int size = input_size - conv->k_size + 1;
	for (int kd = 0; kd < conv->depth; kd++) {														//number of feature					
		float* kernel3D = conv->kernel + (conv->k_size * conv->k_size * input_depth) * kd;
		float* output3D = conv->feature + (conv->f_size * conv->f_size) * kd;
		float* tmp = (float*)calloc(conv->f_size * conv->f_size, sizeof(float));						//�ӽ÷� output������ �����صδ� ��
		for (int d = 0; d < input_depth; d++) {
			float* input2D = input3D + input_size * input_size * d;
			float* kernel2D = kernel3D + conv->k_size * conv->k_size * d;
			for (int y = 0; y < size; y++) {																		//size*size ũ���� output�� �� ������ ����� �Ѵ�.
				for (int x = 0; x < size; x++) {
					for (int ky = 0; ky < conv->k_size; ky++) {														//�̶� �� ���� kernel_size * kernel_size ���� ������ ���� ���
						for (int kx = 0; kx < conv->k_size; kx++) {
							////////////////////////////////////////////////////////////////////////////////
							//
							//
							// PROBLEM 3: ADD YOUR CODE BELOW
							//
							// �־��� �ǽ��� Pooling layer������ 2 x 2 filter Max Pooling�� ����մϴ�.
							// �� ������ ������ �Ʒ��� ��ĭ �ڵ带 �ۼ��ϼ���.
							//
							//
							////////////////////////////////////////////////////////////////////////////////
							/* ADD YOUR CODE HERE */
							tmp[y * conv->f_size + x] += input2D[(y + ky) * input_size + x + kx] * kernel2D[ky * conv->k_size + kx];
						}
					}
				}
			}
		}
		memcpy(output3D, tmp, conv->f_size * conv->f_size * sizeof(float));
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				output3D[y * size + x] = Sigmoid(output3D[y * size + x]);
			}
		}
	}
}
inline void Convolution3D_B(ConvLayer* conv, PoolLayer* pool) {
	/*
	*	@TODO
	*	������Ƿ��̾��� ������ Ǯ�����̾��� ������ ����.
	*	������� ���ĸ� ����� ������ ������ ����� Ǯ���� ������ ���.
	*	@tip
	*	Convolutiuon �� ���������� 6�� ����.
	*/

	memset(pool->error, 0, pool->f_size * pool->f_size * pool->depth * sizeof(float));

	for (int kd = 0; kd < conv->depth; kd++) {
		float* c_error3D = conv->error + (conv->f_size * conv->f_size) * kd;
		float* kernel3D = conv->kernel + (conv->k_size * conv->k_size * pool->depth) * kd;
		for (int d = 0; d < pool->depth; d++) {
			float* p_error2D = pool->error + pool->f_size * pool->f_size * d;
			float* kernel2D = kernel3D + conv->k_size * conv->k_size * d;
			for (int y = 0; y < pool->f_size; y++) {
				for (int x = 0; x < pool->f_size; x++) {
					for (int ky = 0; ky < conv->k_size; ky++) {
						for (int kx = 0; kx < conv->k_size; kx++) {
							int x_index = x + kx - conv->k_size + 1;									//conv layer�� error������ x index
							int y_index = y + ky - conv->k_size + 1;									//conv layer�� error������ y index
							if (x_index >= 0 && x_index < conv->f_size && y_index >= 0 && y_index < conv->f_size) {	//conv layer error�� index ���� üũ
								p_error2D[(y)*pool->f_size + (x)] += kernel2D[(conv->k_size - 1 - ky) * conv->k_size + (conv->k_size - 1 - kx)] * c_error3D[y_index * conv->f_size + x_index];
							}
						}
					}
				}
			}
		}
	}
}

inline void Convolution3D_U(ConvLayer* conv, float* input3D, int input_size, int input_depth, float learning_rate) {
	/*
	*	@TODO
	*	MLP�� ����ġ ������Ʈ�� �����.
	*	������� ���̾��� ����, �̺е� ��������� ��, ���� ���̾��� ��°��� �ʿ���.
	*/
	for (int kd = 0; kd < conv->depth; kd++) {
		float* c_error3D = conv->error + (conv->f_size * conv->f_size) * kd;
		float* c_feature3D = conv->feature + (conv->f_size * conv->f_size) * kd;
		float* kernel3D = conv->kernel + (conv->k_size * conv->k_size * input_depth) * kd;
		for (int d = 0; d < input_depth; d++) {
			float* kernel2D = kernel3D + conv->k_size * conv->k_size * d;
			float* input2D = input3D + input_size * input_size * d;
			for (int ky = 0; ky < conv->k_size; ky++) {
				for (int kx = 0; kx < conv->k_size; kx++) {
					float m = 0.0F;
					for (int y = 0; y < conv->f_size; y++) {
						for (int x = 0; x < conv->f_size; x++) {
							m += (c_error3D[y * conv->f_size + x]) * input2D[(y + ky) * input_size + (x + kx)];
						}
					}
					kernel2D[ky * conv->k_size + kx] += learning_rate * m;	//��� kernel�� ���ҿ� ���ؼ� ������ ������ ��ģ input�� convolution�� gradient�� ���� ������ ����
				}
			}
		}
	}
}
inline void SubSampling(ConvLayer* conv, PoolLayer* pool) {
	/*
	*	@TODO
	*	MAX-POOLING�� ����.
	*/
	for (int d = 0; d < conv->depth; d++) {
		float* input = conv->feature + conv->f_size * conv->f_size * d;
		float* output = pool->feature + pool->f_size * pool->f_size * d;
		for (int y = 0; y < conv->f_size; y += 2) {
			for (int x = 0; x < conv->f_size; x += 2) {
				float* p = &output[(y / 2) * pool->f_size + (x / 2)];
				*p = input[(y + 0) * conv->f_size + (x + 0)];
				*p = MAX(*p, input[(y + 0) * conv->f_size + (x + 1)]);
				*p = MAX(*p, input[(y + 1) * conv->f_size + (x + 0)]);
				*p = MAX(*p, input[(y + 1) * conv->f_size + (x + 1)]);
			}
		}
	}
}
inline void UpSampling(PoolLayer* pool, ConvLayer* conv) {
	/*
	*	@TODO
	*	�ִ� Ǯ���� ������ ������� ������ ����. ���� ū ���� ������� �����ʸ� �״�� �����ϰ� �������� 0���� ����.
	*/
	for (int d = 0; d < pool->depth; d++) {
		float* perror = pool->error + pool->f_size * pool->f_size * d;						//error ����
		float* pfeature = pool->feature + pool->f_size * pool->f_size * d;
		float* cerror = conv->error + conv->f_size * conv->f_size * d;
		float* cfeature = conv->feature + conv->f_size * conv->f_size * d;
		for (int y = 0; y < conv->f_size; y += 2) {
			for (int x = 0; x < conv->f_size; x += 2) {
				int my = y;
				int mx = x;
				float m = -FLT_MAX;
				for (int px = 0; px < 2; px++) {
					for (int py = 0; py < 2; py++) {
						cerror[(y + py) * conv->f_size + (x + px)] = 0.0F;
						if (cfeature[(y + py) * conv->f_size + (x + px)] > m) {
							m = cfeature[(y + py) * conv->f_size + (x + px)];
							my = y + py;
							mx = x + px;
						}
					}
				}
				cerror[(my)*conv->f_size + (mx)] = (perror[(y / 2) * pool->f_size + (x / 2)]);
				cerror[(my)*conv->f_size + (mx)] *= Sigmoid_Derivative(cfeature[(my)*conv->f_size + (mx)]);
			}
		}
	}
}
inline int ForwardCNN(CNN* cnn, float* input) {
	cnn->input = input;
	int size = cnn->input_size;
	int depth = cnn->input_depth;
	for (int i = 0; i < cnn->num_of_convolution; i++) {
		Convolution3D(input, size, depth, &cnn->conv[i]);
		SubSampling(&cnn->conv[i], &cnn->pool[i]);
		input = cnn->pool[i].feature;
		size = cnn->pool[i].f_size;
		depth = cnn->pool[i].depth;
	}
	return Forward(&cnn->network, input);
}
inline void BackwardCNN(CNN* cnn, int label, float learning_rate) {
	Backward(&cnn->network, label, learning_rate);
	//FC�Է��� ������ ������ Ǯ������ ������ ������ �����Ѵ�.
	if (cnn->num_of_convolution > 0) {
		memcpy(cnn->pool[cnn->num_of_convolution - 1].error, cnn->network.layers[0].error, cnn->network.layers[0].n * sizeof(float));
	}
	for (int i = cnn->num_of_convolution - 1; i >= 0; i--) {
		UpSampling(&cnn->pool[i], &cnn->conv[i]);
		if (i != 0) {
			Convolution3D_B(&cnn->conv[i], &cnn->pool[i - 1]);
		}
	}
	float* input = cnn->input;
	int input_size = cnn->input_size;
	int input_depth = cnn->input_depth;
	for (int i = 0; i < cnn->num_of_convolution; i++) {
		Convolution3D_U(&cnn->conv[i], input, input_size, input_depth, learning_rate);
		input = cnn->pool[i].feature;
		input_size = cnn->pool[i].f_size;
		input_depth = cnn->pool[i].depth;
	}
}
inline void WriteParameters(CNN* cnn, char* file) {
	FILE* fp = fopen(file, "w");
	for (int i = 0; i < cnn->num_of_convolution; i++) {
		for (int j = 0; j < cnn->conv[i].k_size * cnn->conv[i].k_size * cnn->conv[i].depth; j++) {
			fprintf(fp, "%f ", cnn->conv[i].kernel[j]);
		}
		fputc('\n', fp);
	}
	for (int i = 0; i < cnn->network.depth; i++) {
		if (i != 0) {
			for (int j = 0; j < cnn->network.layers[i - 1].n * cnn->network.layers[i].n; j++) {
				fprintf(fp, "%f ", cnn->network.layers[i].w[j]);
			}
			fputc('\n', fp);
		}
	}
	fclose(fp);
}
inline void ReadParameters(CNN* cnn, char* file) {
	FILE* fp = fopen(file, "r");
	if (fp == NULL) {

		return;
	}
	for (int i = 0; i < cnn->num_of_convolution; i++) {
		for (int j = 0; j < cnn->conv[i].k_size * cnn->conv[i].k_size * cnn->conv[i].depth; j++) {
			fscanf(fp, "%f ", &cnn->conv[i].kernel[j]);
		}
	}
	for (int i = 0; i < cnn->network.depth; i++) {
		if (i != 0) {
			for (int j = 0; j < cnn->network.layers[i - 1].n * cnn->network.layers[i].n; j++) {
				fscanf(fp, "%f ", &cnn->network.layers[i].w[j]);
			}
		}
	}
	fclose(fp);
}
#endif