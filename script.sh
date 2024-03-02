PROJECT="wp_test"
cd sw/applications/${PROJECT}/kernels/conv/
python3 render-c.py
cd ../../../../../
cd sw/applications/${PROJECT}/utils/
python3 inst_encoder.py ../kernels/conv/ 4x4
cd ../../../../
cd sw/applications/${PROJECT}/kernels/conv/
python3 bitstream.py
cd ../../../../../
