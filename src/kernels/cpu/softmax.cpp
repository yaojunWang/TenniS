#include <kernels/cpu/softmax.h>
#include <core/tensor_builder.h>
#include <algorithm>
#include <math.h>

namespace ts {

	void Softmax::init()
	{
		supper::init();
		if(!has("dim"))
			throw ts::Exception("Missing dim parameter");
		auto dim_tensor = get("dim");
		m_dim = tensor::to_int(dim_tensor);

		if (m_dim < 0)
			throw ts::Exception("Softmax dim should be greater than or equale to 0!");
	}

	int Softmax::run(ts::Stack &stack)
	{
		std::vector<ts::Tensor::Prototype> output;

		this->infer(stack, output);
		stack.push(output[0], memory_device());

		auto dtype = stack.index(0)->dtype();
		bool flag;
		switch (dtype)
		{
			case ts::FLOAT32:
			{
				flag = softmax<float>(stack);
				break;
			}
			case ts::FLOAT64:
			{
				flag = softmax<double>(stack);
				break;
			}
			default:
			{
				throw ts::Exception("Softmax only support FLOAT32 and FLOAT64 type");
				break;
			}
		}
		if (!flag)
			throw ts::Exception("Softmax failed!");
		return 1;
	}

	int Softmax::infer(ts::Stack &stack, std::vector<ts::Tensor::Prototype> &output)
	{
		int input_num = stack.size();

		if (input_num != 1)
			throw ts::Exception("Input parameter should be one!");

		if (stack.index(0)->dtype() != FLOAT32 && stack.index(0)->dtype() != FLOAT64)
			throw ts::Exception("Input parameter should be float or double");;

		if (m_dim < 0 || m_dim >= stack.index(0)->dims()) 
		{
			throw ts::Exception("Softmax dim parameter check failed");
		}

		output.resize(1);
		output[0] = ts::Tensor::Prototype(stack.index(0)->dtype(), stack.index(0)->sizes());

		return 1;
	}

	template<typename T>
	bool Softmax::softmax(ts::Stack &stack)
	{
		ts::Tensor& output_tensor = *stack.index(-1);
		auto output_shape = output_tensor.sizes();

		int pre_num = 1;
		for (int i = 0; i < m_dim; i++)
		{
			pre_num *= output_shape[i];
		}
		int inner_num = 1;
		for (int i = m_dim+1; i < output_shape.size(); i++)
		{
			inner_num *= output_shape[i];
		}

		int axis = output_shape[m_dim];
		T* input_data = stack.index(0)->sync(memory_device()).data<T>();
		T* output_data = output_tensor.sync(memory_device()).data<T>();
		if (output_data == nullptr || input_data == nullptr)
			return false;

		std::memcpy(output_data,input_data, output_tensor.count() * sizeof(T));
		int scale_data_size = output_tensor.count() / axis;
		T* scale_data = new T[scale_data_size];
		T* denominator_data = new T[scale_data_size];
		for (int i = 0; i < pre_num; i++)
		{
			std::memset(denominator_data, 0, scale_data_size * sizeof(T));
			//Caculate max value
			std::memcpy(scale_data,input_data + i * axis * inner_num, inner_num * sizeof(T));
			for (int j = 0; j < axis; j++)
			{
				for (int k = 0; k < inner_num; k++)
				{
					scale_data[k] = std::max(scale_data[k], input_data[i*axis*inner_num + j*inner_num + k]);
				}
			}
			//Caculate numerator and denominator
			for (int j = 0; j < axis; j++)
			{
				for (int k = 0; k < inner_num; k++)
				{
					output_data[i*axis*inner_num + j*inner_num + k] = output_data[i*axis*inner_num + j*inner_num + k] - scale_data[k];
					output_data[i*axis*inner_num + j*inner_num + k] = exp(output_data[i*axis*inner_num + j*inner_num + k]);
					denominator_data[k] += output_data[i*axis*inner_num + j*inner_num + k];
				}
			}
			//Caculte output
			for (int j = 0; j < axis; j++)
			{
				for (int k = 0; k < inner_num; k++)
				{
					output_data[i*axis*inner_num + j*inner_num + k] = output_data[i*axis*inner_num + j*inner_num + k] / denominator_data[k];
				}
			}
			
		}
		delete[] scale_data;
		delete[] denominator_data;
		
		return true;
	}
	TS_REGISTER_OPERATOR(Softmax, ts::CPU, "softmax")
}