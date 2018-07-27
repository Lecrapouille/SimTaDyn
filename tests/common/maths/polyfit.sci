// ===========================================================================
// polyfit:
// Original version:
// https://forge.scilab.org/index.php/p/stixbox/source/tree/master/macros/polyfit.sci
// Copyright (C) 2013 - 2016 - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
// Copyright (C) 1993 - 1995 - Anders Holtsberg
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// ===========================================================================
function [p,cr] = polyfit(x,y,n)
  if prod(size(x))<>prod(size(y))  then
    error('x and y  must be with the same size')
  end

  if(size(x,1) == 1) then x=x';end;
  if(size(y,1) == 1) then y=y';end;
  nx=prod(size(x));ny=prod(size(y));

  vander = zeros(nx,n+1);
  vander(:,n+1) = ones(nx,1);
  for j = n:-1:1,
    vander(:,j) = x.*vander(:,j+1);
  end;
  [Q,R] = qr(vander,'e');
  Q = Q(:,1:size(R,2))
  R = R(1:size(R,2),:)
  p = R\(Q'*y);
  r = y - vander*p; //residuals
  p = p($:-1:1);    //cofficients of the polynomial
  p=p';
  freed = ny - (n+1); //degree of freedom

  //on return : choleski factor and the norm of residuals
  cr = [R; [freed zeros(1,n)]; [norm(r) zeros(1,n)]];
endfunction
