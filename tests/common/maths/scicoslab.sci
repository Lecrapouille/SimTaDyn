//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

// ===========================================================================
// Input signal
t=-%pi:0.05:%pi;
u=-10-20*sin(5*t).*exp(-t/2);

// ===========================================================================
function y=LowPassFilter(fc, h, u)
  s=poly(0, "s");
  wc=2*%pi*fc;
  Hc=syslin('c',1/(1+s/wc));
  Hd=dscr(tf2ss(Hc), h);
  y=dsimul(Hd, u)
endfunction

y=LowPassFilter(2, 1/200, u);
fprintfMat("./generated/LowPassFilter1storder.h", [u', y'], '%.20f,');

// ===========================================================================
// Equivalent to the following code but it's faster
// Y=U;
// for i = 1:size(X,1)
//  Y(i) = mean(X(max([1 i-Nb_pt_filt]):i,1));
// end
Nb_pt_filt=64;
function Mf=FiltMean(X)
  s=size(X,1);
  Mf=zeros(1,s);
  for i = 1:Nb_pt_filt
    Mf(i) = mean(X(1:i));
  end
  for i = Nb_pt_filt+1:s
    Mf(i) = Mf(i-1) + (X(i) - X(i-Nb_pt_filt)) / Nb_pt_filt;
  end
endfunction

//y=FiltMean(u');
X=u';
y=u';
for i=1:size(X,1)
y(i)=mean(X(max([1 i-Nb_pt_filt]):i,1));
end
fprintfMat("./generated/RollingAverageFilter.h", [u', y], '%.20f,');

exec('polyfit.sci');


x=[1:10];
y=x^3+4*x^2+(rand([1:10])*50);
// plot2d(x, y, style=1);
[p,cr] = polyfit(x,y, 3);
z = p(1)+p(2)*x+p(3)*x^2+p(4)*x^3;
//plot2d(x, z, style=2);
fprintfMat("./generated/PolyFitIO.h", [x', y'], '%.20f,');
fprintfMat("./generated/PolyFit.h", [p'], '%.20f,');
