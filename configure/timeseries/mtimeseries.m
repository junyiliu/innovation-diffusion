files=dir('*.txt');
n=length(files);
color='rgbcm';
for i=1:4 
	subplot(2,2,i)
	for j=1:5;
		a=load(files((i-1)*4+j).name);
		plot(a,color(j))
		hold on;
		%legend(['b=',num2str(j-3)]);
        %legend on
	end
	xlabel('t','FontSize',30);
	ylabel('I(t)','FontSize',30);
	text(390,5,['(',num2str(i),')']);
    if i==1
        legend('b=-2','b=-1','b=0','b=1','b=2')
    end
end 