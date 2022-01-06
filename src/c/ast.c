#include <stdlib.h>
#include <string.h>

#include "ast.h"

CVAR * cvar_alloc(char *name, uint16_t value, int is_addr)
{
	CVAR *var = (CVAR *)calloc(1, sizeof(CVAR));

	memcpy(&var->name, name, strlen(name));

	if (is_addr)
		var->addr = value;
	else
		var->value = (uint8_t)value;

	return var;
}

void cvar_free(CVAR *var)
{
	free(var);
	var = NULL;
}

CFUNC * cfunc_alloc(char *name)
{
	CFUNC *func = (CFUNC *)calloc(1, sizeof(CFUNC));
	memcpy(&func->name, name, strlen(name));

	return func;
}

void cfunc_add_arg(CVAR *var)
{
	if (!func->args)
		func->args = (CVNODE *)calloc(1, sizeof(CVNODE));

	CVNODE *it = func->args;

	while (it->next)
		it = it->next;

	it = (CVNODE *)calloc(1, sizeof(CVNODE));
	it->var = var;
}

void cfunc_free(CFUNC *func)
{
	if (!func) return;

	CVNODE *it = NULL;
	CVNODE *next = func->args;

	while (next)
	{
		it = next;
		next = it->next;
		free(it);
	}

	free(func);
	func = NULL;
}

CSTREE * cstree_alloc()
{

}

void cstree_free(CSTREE *tree)
{
	if (!tree) return;

	CFNODE *fit = NULL;
	CFNODE *fnext = tree->funcs;
	CVNODE *vit = NULL;
	CVNODE *vnext = tree->vars;

	while (fnext)
	{
		fit = fnext;
		fnext = fit->next;
		free(fit);
	}

	while (vnext)
	{
		vit = vnext;
		vnext = vit->next;
		free(vit);
	}

	free(tree);
	tree = NULL;
}
