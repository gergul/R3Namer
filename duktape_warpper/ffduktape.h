#ifndef __FFDUKTAPE
#define __FFDUKTAPE

#include <duktape.h>
#include <stdexcept>
#include <vector>

//DEKTAPE JS FUNCTION HELPER
#define GET_ARGC(ctx) (duk_get_top(ctx))
#define RETURN_STR(ctx, str) {duk_push_string((ctx), (str));return 1;  /* one return value */}
#define RETURN_NUM(ctx, num) {duk_push_number((ctx), (duk_double_t)(num));return 1;  /* one return value */}
#define RETURN_VOID() {return 0; /* no return value */}

namespace JS
{
	class Context
	{
	public:
		Context(
			duk_alloc_function alloc_func = NULL,
			duk_realloc_function realloc_func = NULL,
			duk_free_function free_func = NULL,
			void *heap_udata = NULL,
			duk_fatal_function fatal_handler = NULL
			)
			: parent(NULL)
			, whereAtParent(-1)
		{
			ctx = duk_create_heap(
				alloc_func,
				realloc_func,
				free_func,
				heap_udata,
				fatal_handler);

			bICreated = true;
		}

		Context(const Context &parent)
		{
			this->operator=(parent);
		}

		Context(duk_context *ctx)
		{
			this->ctx = ctx;

			bICreated = false;
		}

		Context &operator=(const Context &parent)
		{
			this->parent = parent.ctx;
			this->whereAtParent = duk_push_thread(parent.ctx);
			if (this->whereAtParent < 0)
			{
				this->ctx = NULL;
				this->parent = NULL;
				this->whereAtParent = -1;
			}
			else
				this->ctx = duk_get_context(parent.ctx, this->whereAtParent);

			bICreated = true;
			return *this;
		}

		~Context()
		{
			if (NULL != ctx && bICreated)
			{
				if (NULL == parent)
					duk_destroy_heap(ctx);
				else if (duk_is_thread(parent, whereAtParent))
					duk_remove(parent, whereAtParent);
			}
		}

		//add c function to js
		//duk_ret_t C_FUNCTION_NAME(duk_context *ctx)
		//{
		//  ...
		//	return 0;  /* number of return values count*/
		//}
		bool addCFunction(const char *funcName, duk_c_function func, duk_idx_t nargs = DUK_VARARGS)
		{
			if (NULL == funcName || NULL == func)
				return false;

			if (duk_push_c_function(ctx, func, nargs) < 0)
				return false;
			if (duk_put_global_string(ctx, funcName) != 1)
			{
				duk_pop(ctx);//undo duk_push_c_function
				return false;
			}

			return true;
		}

		// eval
		template<typename RET_T=void>
		RET_T eval(const char *src)
		{
			_eval(src);

			RET_T ret = to<RET_T>();
			duk_pop(ctx);
			return ret;
		}

		// evalFile
		template<typename RET_T=void>
		RET_T evalFile(const char *pFile)
		{
			return eval_file<RET_T>(ctx, pFile);
		}

		template<typename T>
		T to()
		{
			//__unsupported_type;
			std::runtime_error ex("unsupported type");
			throw ex;
			return (T)0;
		}

		// operator<<
		template<typename T>
		Context &operator<<(const T &arg)//char *
		{
			duk_push_string(ctx, (const char *)arg);
			return *this;
		}
		template<typename T>
		Context &operator<<(const std::vector<T> &arg)
		{
			duk_idx_t arr_idx = duk_push_array(ctx);
			int nSize = arg.size();
			for (int i = 0; i < nSize; ++i)
			{
				this->operator<<(arg[i]);
				duk_put_prop_index(ctx, arr_idx, i);
			}

			return *this;
		}

		// operator>>
		template<typename T>
		Context &operator>>(T &arg)//char *
		{
			duk_safe_to_string(ctx, -1);
			duk_size_t out = 0;
			const char *p = duk_get_lstring(ctx, -1, &out);
			if (NULL == p)
				return *this;

			char *o = (char *)arg;
			strcpy(o, p);

			duk_pop(ctx);

			return *this;
		}
		// 	template<typename T>
		// 	Context &operator>>(std::vector<T> &arg)
		// 	{
		// 		while (!duk_is_array(ctx))
		// 		{
		// 
		// 		}
		// 		duk_idx_t arr_idx = duk_push_array(ctx);
		// 		int nSize = arg.size();
		// 		for (int i = 0; i < nSize; ++i)
		// 		{
		// 			this->operator<<(arg[i]);
		// 			duk_put_prop_index(ctx, arr_idx, i);
		// 		}
		// 
		// 		return *this;
		// 	}

	public:
		duk_context *ctx;

	protected:
		void _eval(const char *src)
		{
			duk_push_string(ctx, src);
			if (duk_peval(ctx) != 0)
			{
				std::runtime_error ex(duk_safe_to_string(ctx, -1));
				duk_pop(ctx);
				throw ex;
			}
		}

		template<typename RET_T>
		RET_T eval_file(duk_context *ctx, const char *path, duk_uint_t flags = 0)
		{
			FILE *f = NULL;
			char *buf;
			long sz;  /* ANSI C typing */
			RET_T ret;

			if (!path) {
				goto fail;
			}
			f = fopen(path, "rb");
			if (!f) {
				goto fail;
			}
			if (fseek(f, 0, SEEK_END) < 0) {
				goto fail;
			}
			sz = ftell(f);
			if (sz < 0) {
				goto fail;
			}
			if (fseek(f, 0, SEEK_SET) < 0) {
				goto fail;
			}
			buf = (char *)new char[(duk_size_t)sz + 1];
			memset(buf, 0, (duk_size_t)sz + 1);

			if ((duk_size_t)fread(buf, 1, (size_t)sz, f) != (duk_size_t)sz) {
				goto fail;
			}
			(void)fclose(f);  /* ignore fclose() error */
			f = NULL;

			ret = eval<RET_T>(buf);
			delete[] buf;

			return ret;

		fail:
			if (f)
			{
				fclose(f);
			}

			return RET_T();
		}

	private:
		duk_context *parent;
		duk_idx_t whereAtParent;

		bool bICreated;
	};
////////////////////////////////////////////////////////////////////////

	template<>
	void Context::eval(const char *src)
	{
		_eval(src);
	}

	template<>
	void Context::evalFile(const char *pFile)
	{
		evalFile<int>(pFile);
	}

	template<>
	std::string Context::to()
	{
		duk_safe_to_string(ctx, -1);
		duk_size_t out = 0;
		const char *p = duk_get_lstring(ctx, -1, &out);
		if (NULL == p || 0 == out)
			return "";
		return p;
	}
	template<>
	int Context::to()
	{
		duk_to_int(ctx, -1);
		return duk_get_int(ctx, -1);
	}
	template<>
	double Context::to()
	{
		duk_to_number(ctx, -1);
		return duk_get_number(ctx, -1);
	}
	template<>
	float Context::to()
	{
		duk_to_number(ctx, -1);
		return (float)duk_get_number(ctx, -1);
	}
	template<>
	bool Context::to()
	{
		duk_to_boolean(ctx, -1);
		return (bool)(duk_get_boolean(ctx, -1) != 0);
	}
	template<>
	unsigned int Context::to()
	{
		duk_to_uint(ctx, -1);
		return duk_get_uint(ctx, -1);
	}
	template<>
	void Context::to()
	{
		
	}

	template<>
	Context &Context::operator<<(const std::string &arg)
	{
		duk_push_string(ctx, (const char *)arg.c_str());
		return *this;
	}
	template<>
	Context &Context::operator<<(const int &arg)
	{
		duk_push_int(ctx, (duk_int_t)arg);
		return *this;
	}
	template<>
	Context &Context::operator<<(const bool &arg)
	{
		duk_push_boolean(ctx, (duk_bool_t)arg);
		return *this;
	}
	template<>
	Context &Context::operator<<(const double &arg)
	{
		duk_push_number(ctx, (duk_double_t)arg);
		return *this;
	}
	template<>
	Context &Context::operator<<(const float &arg)
	{
		duk_push_number(ctx, (duk_double_t)arg);
		return *this;
	}
	template<>
	Context &Context::operator<<(const unsigned int &arg)
	{
		duk_push_uint(ctx, (duk_uint_t)arg);
		return *this;
	}

	template<>
	Context &Context::operator>>(std::string &arg)
	{
		duk_safe_to_string(ctx, -1);
		duk_size_t out = 0;
		const char *p = duk_get_lstring(ctx, -1, &out);
		if (NULL == p || 0 == out)
			arg = "";
		else
			arg = p;

		duk_pop(ctx);

		return *this;
	}
	template<>
	Context &Context::operator>>(int &arg)
	{
		duk_to_int(ctx, -1);
		arg = duk_get_int(ctx, -1);

		duk_pop(ctx);

		return *this;
	}
	template<>
	Context &Context::operator>>(bool &arg)
	{
		duk_to_boolean(ctx, -1);
		arg = (duk_get_boolean(ctx, -1) != 0);

		duk_pop(ctx);

		return *this;
	}
	template<>
	Context &Context::operator>>(double &arg)
	{
		duk_to_number(ctx, -1);
		arg = duk_get_number(ctx, -1);

		duk_pop(ctx);

		return *this;
	}
	template<>
	Context &Context::operator>>(float &arg)
	{
		duk_to_number(ctx, -1);
		arg = (float)duk_get_number(ctx, -1);

		duk_pop(ctx);

		return *this;
	}
	template<>
	Context &Context::operator>>(unsigned int &arg)
	{
		duk_to_uint(ctx, -1);
		arg = duk_get_uint(ctx, -1);

		duk_pop(ctx);

		return *this;
	}
}
#endif // !__FFDUKTAPE
