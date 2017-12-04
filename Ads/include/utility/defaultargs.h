


#ifndef VARDICDEFS_OR_DEFAULTARGS_H
#define VARDICDEFS_OR_DEFAULTARGS_H


#define varad_head(type,name)  type varadic##name(varadic_type_##name x)

#define varad_declare(type,name,...) \
					typedef struct { \
					__VA_ARGS__ ; \
					}varadic_type_##name;\
					varad_head(type,name);

#define varad_var(name,value) name = (x.name) ? x.name : (value);

#define varad_link(name,...) varadic##name((varadic_type_##name) {__VA_ARGS__})

/*constructor*/

#define constructor_actual_function_head(type,name)  type varadic_function_##name(int datatype,varadic_type_##name x)

#define constructor_prototype_declare(type,name,...) \
					typedef struct { \
					char safe;\
					__VA_ARGS__ ; \
					}varadic_type_##name;\
					constructor_actual_function_head(type,name)
					
#define constructor_default_argument_fvar(name,args,value) (*name)args = (x.name) ? x.name : (value);
#define constructor_default_argument_var(name,value) name = (x.name) ? x.name : (value);

#define constructor_link(datatype,name,...) varadic_function_##name(datatype,(varadic_type_##name) {0,__VA_ARGS__})



#endif

