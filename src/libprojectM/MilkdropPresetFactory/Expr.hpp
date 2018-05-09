/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2007 projectM Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */
/**
 * $Id$
 *
 * Expression
 *
 * $Log$
 */

#ifndef _EXPR_H
#define _EXPR_H

#include "dlldefs.h"
#include "CValue.hpp"

class Param;

#define CONST_STACK_ELEMENT 0
#define EXPR_STACK_ELEMENT 1

#define EVAL_ERROR -1

/* Infix Operator Function */
class InfixOp
{
public:
  int type;
  int precedence;

  InfixOp( int type, int precedence );
};

/** Term */
class Term
{
public:
  float constant; /* static variable */
  Param *param; /* pointer to a changing variable */

  Term() { this->constant = 0; this->param = 0; }
};
 
class Expr
{
public:
  virtual ~Expr() {};
  virtual bool isConstant(int mesh_i, int mesh_j) { return false; };
  virtual float eval(int mesh_i, int mesh_j) = 0;

  static Expr *const_to_expr( float val );
  static Expr *param_to_expr( Param *param );
  static Expr *prefun_to_expr( float (*func_ptr)(void *), Expr **expr_list, int num_args );
};

/* General Expression Type
class GenExpr : public GenExpr
{
public:
  int type;
  Expr * item;

  ~GenExpr();

  GenExpr( int type, Expr *item );
  float eval(int mesh_i, int mesh_j);
  float eval_gen_expr(int mesh_i, int mesh_j) { return eval(mesh_i, mesh_j); }
 };  */

/* Value expression, contains a term union */
class ValExpr : public Expr
{
protected:
  ValExpr( int type, Term *term );
public:
  int type;
  Term term;

  static ValExpr *create( int type, Term *term );
  ~ValExpr();
  
  float eval_val_expr(int mesh_i, int mesh_j) { return eval(mesh_i, mesh_j); }
};

/* A binary expression tree ordered by operator precedence */
class TreeExpr : public Expr
{
private:
  TreeExpr( InfixOp *infix_op, Expr *gen_expr,
                                  TreeExpr *left, TreeExpr *right );
public:
  static TreeExpr *create( InfixOp *infix_op, Expr *gen_expr,
                                  TreeExpr *left, TreeExpr *right );
  InfixOp * infix_op; /* null if leaf */
  Expr * gen_expr;
  TreeExpr *left, *right;

  ~TreeExpr();
  
  float eval(int mesh_i, int mesh_j);
  float eval_tree_expr(int mesh_i, int mesh_j) { return eval(mesh_i, mesh_j); }
};

/* A function expression in prefix form */
class PrefunExpr : public Expr
{
public:
  float (*func_ptr)(void*);
  int num_args;
  Expr **expr_list;
  PrefunExpr();
  ~PrefunExpr();

  /* Evaluates functions in prefix form */
  float eval(int mesh_i, int mesh_j);
  float eval_prefun_expr(int mesh_i, int mesh_j) { return eval(mesh_i, mesh_j); }
};

#endif /** _EXPR_H */
