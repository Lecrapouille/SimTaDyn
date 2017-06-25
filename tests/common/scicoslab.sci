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


x=[1:10];
y=x^3+4*x^2+(rand([1:10])*50);
// plot2d(x, y, style=1);
[p,cr] = polyfit(x,y, 3);
z = p(1)+p(2)*x+p(3)*x^2+p(4)*x^3;
//plot2d(x, z, style=2);
fprintfMat("./generated/PolyFitIO.h", [x', y'], '%.20f,');
fprintfMat("./generated/PolyFit.h", [p'], '%.20f,');
