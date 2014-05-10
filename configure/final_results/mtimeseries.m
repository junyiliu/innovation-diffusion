files=dir('*.txt');
n=length(files);
color='rgbcm';
for i=1:4 
	subplot(2,2,i)
		a=load(files(i).name);
		plot(a(:,1),a(:,2),'r--o')
		hold on;
		%legend(['b=',num2str(j-3)]);
        %legend on
	xlabel('b','FontSize',30);
	ylabel('\rho','FontSize',30);
	text(6,0,['(',num2str(i),')']);
end 