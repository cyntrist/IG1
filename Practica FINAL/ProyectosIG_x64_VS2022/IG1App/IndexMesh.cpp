#include "IndexMesh.h"
#include "CheckML.h"
#include <iostream>
#include <string>

glm::vec3 IndexMesh::calculoVectorNormalPorNewell(Cara c)
{
	glm::vec3 n = {0, 0, 0};
	for (int i = 0; i < c.numeroVertices; i++)
	{
		/*
	      Set Vertex Current to Polygon.verts[Index]
	      Set Vertex Next    to Polygon.verts[(Index plus 1) mod Polygon.vertexNumber]

	      Set Normal.x to Sum of Normal.x and (multiply (Current.y minus Next.y) by (Current.z plus Next.z))
	      Set Normal.y to Sum of Normal.y and (multiply (Current.z minus Next.z) by (Current.x plus Next.x))
	      Set Normal.z to Sum of Normal.z and (multiply (Current.x minus Next.x) by (Current.y plus Next.y))
		 **/

		/*std::cout << c.getIndiceVertice(c.vertices[i], vVertices) << std::endl;
		std::cout << c.getIndiceVertice(c.vertices[(i + 1) % c.numeroVertices]) << std::endl;

		const auto vertActual = vVertices[c.getIndiceVertice(c.vertices[i], vVertices)];
		const auto vertSiguiente = vVertices[c.getIndiceVertice(c.vertices[(i + 1) % c.numeroVertices])];*/

		std::cout << c.getIndice(i) << std::endl;
		std::cout << c.getIndice((i + 1) % c.numeroVertices) << std::endl;

		const auto vertActual = vVertices[c.getIndice(i)];
		const auto vertSiguiente = vVertices[c.getIndice((i + 1) % c.numeroVertices)];

		n.x += (vertActual.y - vertSiguiente.y) * (vertActual.z + vertSiguiente.z);
		n.y += (vertActual.z - vertSiguiente.z) * (vertActual.x + vertSiguiente.x);
		n.z += (vertActual.x - vertSiguiente.x) * (vertActual.y + vertSiguiente.y);
	}
	return normalize(n);
}

void IndexMesh::buildNormalVectors()
{
	int i = 0;
	vNormals.resize(mNumVertices);
	for (int i = 0; i < mNumVertices; i++)
		vNormals.emplace_back(0, 0, 0);
	for (const auto cara : vCaras)
	{
		vNormals[i] = calculoVectorNormalPorNewell(cara);
		i++;
	}
}

void IndexMesh::buildNormalVectorsV2() // version de la profe
{
	vNormals.resize(mNumVertices);

	std::vector<glm::dvec3> vAuxNormals = vNormals; // vector auxiliar

	for (int i = 0; i < nNumIndices / 3; i++)
	{
		glm::dvec3 v0 = vVertices[vIndices[i * 3]];
		glm::dvec3 v1 = vVertices[vIndices[i * 3 + 1]];
		glm::dvec3 v2 = vVertices[vIndices[i * 3 + 2]];

		glm::dvec3 v = v1 - v0;
		glm::dvec3 w = v2 - v0;

		const glm::dvec3 n = normalize(cross(v, w));


		vAuxNormals[vIndices[i * 3]] += n;
		vAuxNormals[vIndices[i * 3 + 1]] += n;
		vAuxNormals[vIndices[i * 3 + 2]] += n;
	}

	for (int i = 0; i < mNumVertices; i++)
		vNormals[i] = normalize(vAuxNormals[i]);
}

IndexMesh* IndexMesh::generateIndexedBox(GLdouble l)
{
	const auto mesh = new IndexMesh();

	/// VERTICES
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);

	mesh->vVertices.emplace_back(l, l, -l); // v0
	mesh->vVertices.emplace_back(l, -l, -l); // v1
	mesh->vVertices.emplace_back(-l, -l, -l); // v2
	mesh->vVertices.emplace_back(-l, l, -l); // v3
	mesh->vVertices.emplace_back(l, l, l); // v4
	mesh->vVertices.emplace_back(l, -l, l); // v5
	mesh->vVertices.emplace_back(-l, -l, l); // v6
	mesh->vVertices.emplace_back(-l, l, l); // v7


	/// INDICES
	/// Define cuidadosamente los 36 índices que, de 3 en 3,
	/// determinan las 12 caras triangulares de la malla.
	/// Recuerda que los índices de estas caras deben darse
	/// en sentido ¡¡¡ANTI-HORARIO!!! según se mira el cubo desde su exterior
	mesh->nNumIndices = 36;
	mesh->vIndices = new GLuint[mesh->nNumIndices];

	// cara de abajo
	mesh->vIndices[0] = 0;
	mesh->vIndices[1] = 3;
	mesh->vIndices[2] = 2;

	mesh->vIndices[3] = 0;
	mesh->vIndices[4] = 2;
	mesh->vIndices[5] = 1;

	// cara de arriba
	mesh->vIndices[6] = 4;
	mesh->vIndices[7] = 5;
	mesh->vIndices[8] = 6;

	mesh->vIndices[9] = 4;
	mesh->vIndices[10] = 6;
	mesh->vIndices[11] = 7;

	// cara lateral cercana izquierda
	mesh->vIndices[12] = 0;
	mesh->vIndices[13] = 4;
	mesh->vIndices[14] = 7;

	mesh->vIndices[15] = 0;
	mesh->vIndices[16] = 7;
	mesh->vIndices[17] = 3;

	// cara lateral trasera derecha
	mesh->vIndices[18] = 1;
	mesh->vIndices[19] = 2;
	mesh->vIndices[20] = 6;

	mesh->vIndices[21] = 1;
	mesh->vIndices[22] = 6;
	mesh->vIndices[23] = 5;

	// cara lateral cercana derecha
	mesh->vIndices[24] = 0;
	mesh->vIndices[25] = 1;
	mesh->vIndices[26] = 5;

	mesh->vIndices[27] = 0;
	mesh->vIndices[28] = 5;
	mesh->vIndices[29] = 4;

	// cara lateral trasera izquierda
	mesh->vIndices[30] = 2;
	mesh->vIndices[31] = 3;
	mesh->vIndices[32] = 7;

	mesh->vIndices[33] = 2;
	mesh->vIndices[34] = 7;
	mesh->vIndices[35] = 6;


	/// COLORES
	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vColors.emplace_back(0, 1, 0, 1);

	/// NORMALES
	mesh->buildNormalVectors();

	return mesh;
}

IndexMesh* IndexMesh::generateIndexedBoxV2(GLdouble l) /// la version de la profe
{
	const auto mesh = new IndexMesh();

	/// VERTICES
	mesh->mNumVertices = 8;
	mesh->vVertices.reserve(mesh->mNumVertices);

	const GLdouble m = l / 2;

	mesh->vVertices.emplace_back(-m, m, m); // v0
	mesh->vVertices.emplace_back(-m, -m, m); // v1
	mesh->vVertices.emplace_back(m, m, m); // v2
	mesh->vVertices.emplace_back(m, -m, m); // v3
	mesh->vVertices.emplace_back(m, m, -m); // v4
	mesh->vVertices.emplace_back(m, -m, -m); // v5
	mesh->vVertices.emplace_back(-m, m, -m); // v6
	mesh->vVertices.emplace_back(-m, -m, -m); // v7

	/// INDICES
	mesh->nNumIndices = 36;
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	const GLuint arr[36] =
	{
		0, 1, 2, 1, 3, 2,
		2, 3, 4, 3, 5, 4,
		4, 5, 6, 5, 7, 6,
		//última diagonal como el resto en la cara lateral izquierda
		//6, 7, 0, 7, 1, 0, // se ve ligeramente distinto, POR?
		//última diagonal al contrario del resto en la cara lateral izquierda
		0, 6, 1, 6, 7, 1,
		0, 2, 4, 4, 6, 0, // arriba
		1, 5, 3, 1, 7, 5 // abajo
	};

	for (int i = 0; i < mesh->nNumIndices; i++)
		mesh->vIndices[i] = arr[i];

	///// CARAS
	//int nV = 3;
	//mesh->vCaras.resize(mesh->nNumIndices / nV);
	//for (int i = 0; i < mesh->nNumIndices / nV; i++)
	//{
	//	//std::cout << mesh->vIndices[i * nV] << std::endl;
	//	//std::cout << mesh->vIndices[i * nV + 1] << std::endl;
	//	//std::cout << mesh->vIndices[i * nV + 2] << std::endl;

	//	mesh->vCaras[i] = Cara(
	//		mesh->vIndices[i * nV],
	//		mesh->vIndices[i * nV + 1],
	//		mesh->vIndices[i * nV + 2]
	//	);
	//}

	/// COLORES
	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vColors.emplace_back(0, 1, 0, 1);


	/// NORMALES
	mesh->buildNormalVectorsV2();

	return mesh;
}

IndexMesh* IndexMesh::generateIndexedDiamond(GLdouble l)
{
	const auto mesh = new IndexMesh();

	/// VERTICES
	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	const GLdouble m = l / 2;

	mesh->vVertices.emplace_back(m, m, 0); // v0
	mesh->vVertices.emplace_back(-m, m, 0); // v1
	mesh->vVertices.emplace_back(-m, -m, 0); // v2
	mesh->vVertices.emplace_back(m, -m, 0); // v3
	mesh->vVertices.emplace_back(0, 0, l); // v4
	mesh->vVertices.emplace_back(0, 0, -l); // v5

	/// INDICES
	mesh->nNumIndices = 24;
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	const GLuint arr[24] =
	{
		0, 1, 4, 
		0, 5, 1, 
		1, 2, 4,
		1, 5, 2, 
		2, 3, 4, 
		2, 5, 3,
		3, 0, 4,
		3, 5, 0
	};

	for (int i = 0; i < mesh->nNumIndices; i++)
		mesh->vIndices[i] = arr[i];

	/// CARAS
	int nV = 3;
	mesh->vCaras.resize(mesh->nNumIndices / nV);
	for (int i = 0; i < mesh->nNumIndices / nV; i++)
	{
		mesh->vCaras[i] = Cara(
			mesh->vIndices[i * nV],
			mesh->vIndices[i * nV + 1],
			mesh->vIndices[i * nV + 2]
		);
	}

	/// COLORES
	mesh->vColors.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vColors.emplace_back(1, 1, 1, 1);


	// creo que con indexed mesh es un lio por como van los indices y tal
	// esta hecho en el normal igual que aqui

	// TEXTURAS
	//mesh->vTexCoords.reserve(mesh->mNumVertices);
	/*for (int i = 0; i < 5; i++) {
		mesh->vTexCoords.emplace_back(0, 0);
		mesh->vTexCoords.emplace_back(0, 0.5);
		mesh->vTexCoords.emplace_back(0.5, 0.5);

		mesh->vTexCoords.emplace_back(0, 0);
		mesh->vTexCoords.emplace_back(0.5, 0.5);
		mesh->vTexCoords.emplace_back(0.5, 0);
	}*/

	/// NORMALES
	mesh->buildNormalVectorsV2();

	return mesh;
}

IndexMesh* IndexMesh::generateIndexedRegularPolygon(GLdouble r, GLint num)
{
	auto* mesh = new IndexMesh();
	//mesh->mNumVertices = num;

	//// numero de vertices
	//const double dividido = 360.0 / num;

	//// angulo inicial
	//constexpr double alpha = glm::radians(90.0);

	////mesh->vVertices.emplace_back(0, 0, 0);
	//for (int i = 0; i < num; i++)
	//{
	//	// x = Cx + R*cos(alpha)
	//	// y = Cy + R*sen(alpha) 
	//	mesh->vVertices.emplace_back(
	//		r * cos(alpha + glm::radians(dividido * i)), 
	//		r * sin(alpha + glm::radians(dividido * i)),
	//		0.0);
	//}

	///*mesh->vNormals.reserve(mesh->mNumVertices);
	//mesh->buildNormalVectorsV2();*/
	return mesh;
}


void IndexMesh::render() const
{
	if (vVertices.empty()) return;

	setGL();
	if (vIndices != nullptr)
	{
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
	}
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_FILL);
	draw();
	//glDisable(GL_COLOR_MATERIAL); // NO HAGAS ESTO!!!!!!!!!!!!!!!!!!!! (yo diciendomelo a mi misma)
	glDisableClientState(GL_INDEX_ARRAY);
	resetGL();
}

void IndexMesh::draw() const
{
	glDrawElements(mPrimitive, nNumIndices,
	               GL_UNSIGNED_INT, vIndices);
}


/// MBR
MbR::MbR(int profPts, int rotNum, glm::dvec3* prof)
	: mProfilePoints(profPts), mRotationNumber(rotNum), mProfile(prof)
{
}

MbR* MbR::generateIndexMbR(GLint mm, GLint nn, glm::dvec3* perfil)
{
	/// PASO 3 
	auto mesh = new MbR(mm, nn, perfil);
	// Definir la primitiva como GL_TRIANGLES
	mesh->mPrimitive = GL_TRIANGLES;
	// Definir el número de vértices como nn*mm
	mesh->mNumVertices = nn * mm;
	// Usar un vector auxiliar de vértices
	//std::vector<glm::vec<3, double>> vs;
	//vs.reserve(mesh->mNumVertices);
	auto vs = new glm::dvec3 [mesh->mNumVertices];

	for (int i = 0; i < nn; i++)
	{
		// Generar la muestra i- ésima de vértices
		GLdouble theta = i * 360 / nn;
		GLdouble c = cos(glm::radians(theta));
		GLdouble s = sin(glm::radians(theta));
		for (int j = 0; j < mm; j++)
		{
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			GLdouble x = c * perfil[j].x + s * perfil[j].z; // anyadido
			int indice = i * mm + j;
			vs[indice] = glm::dvec3(x, perfil[j].y, z);
		}
	}

	/// PASO 4
	//mesh->vVertices.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vVertices.push_back(vs[i]);
	delete[] vs;

	/// PASO 5
	int indiceMayor = 0;
	mesh->nNumIndices = mesh->mNumVertices * 6;
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	for (int i = 0; i < mesh->mNumVertices * 6; i++)
		mesh->vIndices[i] = 0;

	/// PASO 6
	// El contador i recorre las muestras alrededor del eje Y
	for (int i = 0; i < nn; i++)
	{
		// El contador j recorre los vértices del perfil ,
		// de abajo arriba . Las caras cuadrangulares resultan
		// al unir la muestra i- ésima con la (i +1)% nn - ésima
		for (int j = 0; j < mm - 1; j++)
		{
			// El contador indice sirve para llevar cuenta
			// de los índices generados hasta ahora . Se recorre
			// la cara desde la esquina inferior izquierda
			const int indice = i * mm + j;
			/// PASO 7
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;

			// Y análogamente se añaden los otros tres índices
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + 1);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
		}
	}
	// Los cuatro índices son entonces :
	// indice, (indice + mm) % (nn * mm), (indice + mm + 1) % (nn * mm), indice + 1

	/// PASO 8
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->buildNormalVectorsV2();
	return mesh;
}

MbR* MbR::generateCutIndexMbR(GLint mm, GLint nn, glm::dvec3* perfil, GLint grados)
{
	
	auto mesh = new MbR(mm, nn, perfil);
	// Definir la primitiva como GL_TRIANGLES
	mesh->mPrimitive = GL_TRIANGLES;
	// Definir el número de vértices como nn*mm
	mesh->mNumVertices = nn * mm;
	mesh->vVertices.reserve(mesh->nNumIndices);
	// Usar un vector auxiliar de vértices
	//std::vector<glm::vec<3, double>> vs;
	//vs.reserve(mesh->mNumVertices);
	auto vs = new glm::dvec3 [mesh->mNumVertices];

	for (int i = 0; i < nn; i++)
	{
		// Generar la muestra i- ésima de vértices
		GLdouble theta = i * grados / (nn-1);
		GLdouble c = cos(glm::radians(theta));
		GLdouble s = sin(glm::radians(theta));
		for (int j = 0; j < mm; j++)
		{
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			GLdouble x = c * perfil[j].x + s * perfil[j].z; // anyadido
			int indice = i * mm + j;
			vs[indice] = glm::dvec3(x, perfil[j].y, z);
		}
	}

	/// PASO 4
	//mesh->vVertices.reserve(mesh->mNumVertices);
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vVertices.push_back(vs[i]);
	delete[] vs;

	/// PASO 5
	int indiceMayor = 0;
	mesh->nNumIndices = mesh->mNumVertices * 6;
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	for (int i = 0; i < mesh->nNumIndices; i++)
		mesh->vIndices[i] = 0;

	/// PASO 6
	// El contador i recorre las muestras alrededor del eje Y
	auto n = nn - 1;
	auto m = mm - 1;

	for (int i = 0; i < n; i++)
	{
		// El contador j recorre los vértices del perfil ,
		// de abajo arriba . Las caras cuadrangulares resultan
		// al unir la muestra i- ésima con la (i +1)% nn - ésima
		for (int j = 0; j < m; j++)
		{
			// El contador indice sirve para llevar cuenta
			// de los índices generados hasta ahora . Se recorre
			// la cara desde la esquina inferior izquierda
			const int indice = i * mm + j;
			/// PASO 7
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;

			// Y análogamente se añaden los otros tres índices
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + 1);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
		}
	}
	// Los cuatro índices son entonces :
	// indice, (indice + mm) % (nn * mm), (indice + mm + 1) % (nn * mm), indice + 1

	/// PASO 8
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->buildNormalVectorsV2();
	return mesh;
}


MbR* MbR::generatePartialIndexMbR(int mm, int nn, int grados, glm::dvec3* perfil)
{

	// mm: numero de puntos del perfil
	// nn: numero de rotaciones
	// perfil: perfil en el plano XY

	MbR* mesh = new MbR(mm, nn, perfil);	//Creamos la malla

	mesh->mPrimitive = GL_TRIANGLES;		//Primitiva [Exclusiva de Mallas Indexadas]
	mesh->mNumVertices = nn * mm;			//N�mero de v�rtices

	mesh->vVertices.reserve(mesh->mNumVertices);

	auto vs = new glm::dvec3[mesh->mNumVertices];	//Vector auxiliar de v�rtices

	for (int i = 0; i < nn; i++)
	{
		//Genera la muestra i-�sima de v�rtices

		GLdouble theta = i * grados / (nn - 1);

		GLdouble c = cos(glm::radians(theta));
		GLdouble s = sin(glm::radians(theta));

		for (int j = 0; j < mm; j++)
		{
			GLdouble z = -s * perfil[j].x + c * perfil[j].z;
			GLdouble x = c * perfil[j].x + s * perfil[j].z;

			int Indice = i * mm + j;
			vs[Indice] = glm::dvec3(x, perfil[j].y, z);
		}
	}

	// 4. Volcar el array auxiliar v�rtices en el array de v�rtices
	for (int i = 0; i < mesh->mNumVertices; i++)
		mesh->vVertices.push_back(vs[i]);
	delete[] vs;

	// 5. Construir los �ndices de las caras triangulares
	int indiceMayor = 0;
	mesh->nNumIndices = (nn - 1) * (mm - 1) * 6;
	mesh->vIndices = new GLuint[mesh->nNumIndices];

	// Inicializamos nIndexes a 0
	/*for (int i = 0; i < mesh->mNumVertices * 6; i++)
	{
		mesh->nIndexes[i] = 0;
	}*/

	// 6. Se rellena nIndexes
	// i recorre las muestras alrededor del eje Y
	//De alguna manera dejabamos alg�n indice suelto
	for (int i = 0; i < nn - 1; i++)
	{
		// j recorre los vertices del perfil
		for (int j = 0; j < mm - 1; j++)
		{
			// El contador indice sirve para llevar cuenta
			// de los índices generados hasta ahora . Se recorre
			// la cara desde la esquina inferior izquierda
			//indice cuenta los indices generados hasta ahora
			const int indice = i * mm + j;
			// PASO 7
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			int n = (indice + mm) % (nn * mm);
			mesh->vIndices[indiceMayor] = n;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;


			// Y análogamente se añaden los otros tres índices
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
			n = (indice + 1) % (nn * mm);
			mesh->vIndices[indiceMayor] = n;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice % (nn * mm);
			indiceMayor++;

			/*
			indice0 , indice1 , indice2 ,
			indice2 , indice3 , indice0
			*/
		}
	}

	// 8. Construir los vectores normales y construir la malla.
	mesh->vNormals.reserve(mesh->mNumVertices);
	mesh->buildNormalVectorsV2();

	/// DEBUG -> Dibuja las normales
	//for (int i = 0; i < mesh->mNumVertices; i++) {
	//	mesh->vColors.emplace_back(mesh->vNormals[i].x, mesh->vNormals[i].y, mesh->vNormals[i].z, 1);
	//}

	return mesh;
}
