cp ../icons-new/*.jpg new
cp ../icons-new/*.jpg new+denoised
rm 9.new.ICO
rm 9.new+denoised.ICO
python makeIco.py new 9.new.ICO
python makeIco.py new+denoised 9.new+denoised.ICO