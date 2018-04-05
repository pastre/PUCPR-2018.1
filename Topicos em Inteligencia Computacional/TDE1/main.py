from skimage  import feature
import numpy as np

import os

from sklearn.externals import joblib

import cv2


CAMINHO_IMAGENS_TREINO = 'Treino/'
CAMINHO_IMAGENS_TESTE = 'Teste/'
CAMINHO_CLASSIFICADOR = 'classifier.joblib.pkl'
def describe(nPontos, raio, image, eps = 1e-7):
 	lbp = feature.local_binary_pattern(image, nPontos,
			raio, method="uniform")

 	(hist, _) = np.histogram(lbp.ravel(),
			bins=np.arange(0, nPontos + 3),
			range=(0,nPontos + 2))
 
 	hist = hist.astype('float')
 	hist /= (hist.sum() + eps)	#Tira a norma

 	return hist

labels = []
def parse_image(path, training):
 	print("Treinando a imagem ", path)
 	img = cv2.imread(path)
 	cz = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
 	hist = describe(24, 8, cz)
 	if training:
 		to_append = path.split('/')[-1][0]
 		labels.append(to_append)
 	print("Pronto")
 	return hist


# data = [parse_image(path, True) for path in paths.list_images(CAMINHO_IMAGENS_TREINO)]
# modelo = LinearSVC(C = 100.0, random_state = 42)
# modelo.fit(data, labels)
# print("Persistindo dados...")
# joblib.dump(modelo, CAMINHO_CLASSIFICADOR, compress = 9)
# print("Pronto")
modelo = joblib.load(CAMINHO_CLASSIFICADOR)
ok = 0; err = 0;
for root, dirs, files in os.walk(CAMINHO_IMAGENS_TESTE):
    for file in files:
        path = root + file
        pred = modelo.predict(parse_image(path, False).reshape(1, -1))
        if(pred[0] == path.split('/')[1][0]):
            ok += 1
        else:
            err += 1
print("OK", ok, "ERR", err)
