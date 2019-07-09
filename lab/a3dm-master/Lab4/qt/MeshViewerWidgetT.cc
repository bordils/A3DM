/* ========================================================================= *
 *                                                                           *
 *                               OpenMesh                                    *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openmesh.org                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenMesh.                                            *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
 *                                                                           *
 * ========================================================================= */

/*===========================================================================*\
 *                                                                           *             
 *   $Revision$                                                         *
 *   $Date$                   *
 *                                                                           *
\*===========================================================================*/

#define OPENMESHAPPS_MESHVIEWERWIDGET_CC

//== INCLUDES =================================================================

#ifdef _MSC_VER
//#  pragma warning(disable: 4267 4311)
#endif

//
#include <iostream>
#include <fstream>
// --------------------
#include <QImage>
#include <QFileInfo>
#include <QKeyEvent>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>
#include <QRadioButton>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDoubleValidator>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QtQuickControls2/QQuickStyle>


// --------------------
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Tools/Utils/Timer.hh>
#include <MeshViewerWidgetT.hh>
//openmesh



using namespace OpenMesh;
using namespace Qt;

#if defined(_MSC_VER)
#  undef min
#  undef max
#endif

using namespace Qt;
//== IMPLEMENTATION ========================================================== 




template <typename M>
bool MeshViewerWidgetT<M>::open_scalar( const char *_filename, IO::Options _opt )
{
/*-------reading the input scalar file----*/
   QString fname = _filename;
   QFile inputFile(fname);
   if (inputFile.open(QIODevice::ReadOnly))
   {
      int i = 0;
      bool initialize = false;

      QTextStream in( &inputFile);
      while (!in.atEnd())
      {

         QString line = in.readLine();
         if (i == 0) n = line.toInt();
         else
         {

            float value = line.toFloat();

            if(!initialize)
                {
                    min_value = value;
                    max_value = value;
                    initialize = true;
                }

            //set min and max
            if (value < min_value) min_value = value;
            else if (value > max_value) max_value = value;


            vec_num.append(value);
            //cntrl
            if(i < 10){std::cout << "linea "<< i << " :" << line.toStdString() <<   std::endl;}
         }
        i++;
      }
      std::cout << " i : "<< i << std::endl;
      std::cout << "creato vettore di dimensione :" << vec_num.size() << " come n*n*n "<< n*n*n <<  std::endl;
      std::cout << "min :" << min_value << " max : "<< max_value <<  std::endl;

      inputFile.close();
      if(vec_num.size() == 0) return false;

      scalar_mesh_uploaded = true;


      //default treshold
      threshold_value = (min_value+max_value)/2;
      //get the threshold from the user
      set_threshold(_opt);
      std::cout << "threshold  :" << threshold_value <<   std::endl;

//      int done = 1;

/*------(1)-------*/
//      bool ok = compute_oct_mesh(_opt);
//      bool ok = compute_cube_mesh(_opt);



 /*------(2)-------*/ //not working :(
//      bool ok = create_ochtaedron(n, unit, new_mesh_name);


      return true;
   }
}


template <typename M>
void MeshViewerWidgetT<M>::animate(OpenMesh::IO::Options _opt)
{
    std::cout << "animation " << std::endl;

    int animationSteps = 10;

    int thresh_step = int((max_value - min_value)/100);
    float middle = (max_value -min_value)/2;

    float startingValue =  middle - 45*thresh_step;
    float endingValue = middle + 45*thresh_step;


    if(!scalar_mesh_uploaded){
        QMessageBox msgBox;
        msgBox.setText("You have to upload a scalar mesh first :( ");
        msgBox.exec();
        return;
    }
    QString label = "Value (" + QString::number(min_value) + ", " + QString::number(max_value) + ") :";

    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);
    // Add some text above the fields
    form.addRow(new QLabel("Animation"));

    QSpinBox *step_in = new QSpinBox(&dialog);
    step_in->setMinimum(0);
    step_in->setMaximum(100);
    step_in->setSingleStep(1);
    step_in->setValue(animationSteps);
    form.addRow(QString("Animation step :"),step_in);


    QSpinBox *start_in = new QSpinBox(&dialog);
    start_in->setMinimum(int(min_value + 1*thresh_step));
    start_in->setMaximum(int(endingValue - 1*thresh_step));
    start_in->setSingleStep(thresh_step);
    start_in->setValue(int(startingValue));
    form.addRow(QString("start at :"),start_in);

    QSpinBox *end_in = new QSpinBox(&dialog);
    end_in->setMinimum(int(startingValue + 1*thresh_step));
    end_in->setMaximum(int(max_value - 1*thresh_step));
    end_in->setSingleStep(thresh_step);
    end_in->setValue(int(endingValue));
    form.addRow(QString("end at :"),end_in);



    form.addRow(new QLabel("Select type:"));


    QRadioButton *radio1 = new QRadioButton(tr("&Minimal Visualization"));
    QRadioButton *radio2 = new QRadioButton(tr("M&arching cubes"));
    if(marching_cube_selected)
        radio2->setChecked(true);
    else
        radio1->setChecked(true);

    form.addRow(radio1);
    form.addRow(radio2);

    QObject::connect(start_in, SIGNAL(valueChanged(int)),end_in,SLOT(setMinimum(int)) );
    QObject::connect(end_in,SIGNAL(valueChanged(int)),start_in,SLOT(setMaximum(int)) );

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));




    if (dialog.exec() == QDialog::Accepted)
    {

        if(radio1->isChecked())
        {
            marching_cube_selected = false;
            std::cout << "minimal visualization selected" << std::endl;
        }
        else if(radio2->isChecked())
        {
            marching_cube_selected = true;
            std::cout << "marching cubes selected" << std::endl;
        }
//        result = in_val->value();
//        threshold_value = result;
        endingValue = end_in->value();
        startingValue = start_in->value();
        animationSteps = step_in->value();

        int end_loop = animationSteps;

        if( int((endingValue - startingValue)/thresh_step) < animationSteps )
            end_loop = int((endingValue - startingValue)/thresh_step);


        float old_threshold_value = threshold_value;
        float step = (endingValue - startingValue)/animationSteps;
        threshold_value = startingValue;
        for (int i = 0; i < animationSteps; i ++)
        {

//            QMessageBox msgBox;
//            msgBox.setText("Animate");
//            msgBox.exec();

//            threshold_value = step*i;
            if(marching_cube_selected)
            {
                compute_mc_mesh(_opt);
            }
             else
            {
                compute_cube_mesh(_opt);
            }

            updateGL();

            threshold_value = threshold_value + step;
            std::cout << "last threshold is not too big " << threshold_value << std::endl;
        }

        threshold_value = old_threshold_value;
        if(marching_cube_selected)
        {
            compute_mc_mesh(_opt);
            updateGL();

        }
        else
        {
            compute_cube_mesh(_opt);
            updateGL();
        }
        return;
    }

    else if(dialog.exec() == QDialog::Rejected)
    {

        if(radio1->isChecked())
        {
            marching_cube_selected = false;
            std::cout << "minimal visualization" << std::endl;
        }
        else if(radio2->isChecked())
        {
            marching_cube_selected = true;
            std::cout << "marching cubes" << std::endl;
        }

        std::cout << "cencel, threshold set by default" << std::endl;
//        threshold_value = old_val;
        return;
    }


}




template <typename M>
void MeshViewerWidgetT<M>::set_threshold(OpenMesh::IO::Options _opt)
{
//    float old_val = threshold_value;
//    QString value_range = "Value (" + QString::number(min_value) + ", " + QString::number(max_value) + ") :";
//    bool ok;
//    double d = QInputDialog::getDouble(this, tr("Set threshold :/ "),
//                                       value_range, old_val , min_value, max_value, 2, &ok);
//    if (!ok)
//        {
//            std::cout << "cancel, returned half value  :" << old_val <<   std::endl;
//            threshold_value = old_val;
//        }

//    threshold_value = d;

    if(!scalar_mesh_uploaded){
        QMessageBox msgBox;
        msgBox.setText("You have to upload a scalar mesh first :( ");
        msgBox.exec();
        return;
    }

    float old_val = threshold_value;
    float result = old_val;
    QString label = "Value (" + QString::number(min_value) + ", " + QString::number(max_value) + ") :";

    QDialog dialog(this);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);
    // Add some text above the fields
    form.addRow(new QLabel("Set threshold"));


//    QDoubleSpinBox *thresh_in = new QDoubleSpinBox(&dialog);
//    thresh_in->setMaximum(max_value);
//    thresh_in->setMinimum(min_value);
//    thresh_in->setSingleStep((max_value-min_value)/100);
//    thresh_in->setValue(old_val);
//    form.addRow(label,thresh_in);

    int step = int((max_value-min_value)/100);

    QSpinBox *in_val = new QSpinBox(&dialog);
    in_val->setMinimum(int(min_value + 1*step));
    in_val->setMaximum(int(max_value - 1*step));
    in_val->setSingleStep( step );
    in_val->setValue(int(old_val));
    form.addRow(label,in_val);

    QSlider *slider = new QSlider(Qt::Horizontal,&dialog);
    slider->setTickPosition(QSlider::TicksAbove);
    slider->setMinimum(int(min_value + 1*step));
    slider->setMaximum(int(max_value - 1*step));
    slider->setSingleStep(step);
    slider->setValue(int(old_val));
    form.addRow(slider);


    form.addRow(new QLabel("Select type:"));


    QRadioButton *radio1 = new QRadioButton(tr("&Minimal Visualization"));
    QRadioButton *radio2 = new QRadioButton(tr("M&arching cubes"));
    if(marching_cube_selected)
        radio2->setChecked(true);
    else
        radio1->setChecked(true);

    form.addRow(radio1);
    form.addRow(radio2);

    QObject::connect(in_val, SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)) );
    QObject::connect(slider,SIGNAL(valueChanged(int)),in_val,SLOT(setValue(int)) );

    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));


    if (dialog.exec() == QDialog::Accepted)
    {

        if(radio1->isChecked())
        {
            marching_cube_selected = false;
            std::cout << "minimal visualization selected" << std::endl;
        }
        else if(radio2->isChecked())
        {
            marching_cube_selected = true;
            std::cout << "marching cubes selected" << std::endl;
        }
        result = in_val->value();
        threshold_value = result;

    }

    else if(dialog.exec() == QDialog::Rejected)
    {

        if(radio1->isChecked())
        {
            marching_cube_selected = false;
            std::cout << "minimal visualization" << std::endl;
        }
        else if(radio2->isChecked())
        {
            marching_cube_selected = true;
            std::cout << "marching cubes" << std::endl;
        }

        std::cout << "cencel, threshold set by default" << std::endl;
        threshold_value = old_val;
    }


    if(marching_cube_selected)
    {
        compute_mc_mesh(_opt);
        updateGL();
    }
    else
    {
        compute_cube_mesh(_opt);
        updateGL();
    }
}

template <typename M>
bool MeshViewerWidgetT<M>::compute_mc_mesh( IO::Options _opt)
{

    int count_faces = 0;

    my_mesh mesh;

    my_mesh::VertexHandle vhandle[3];
    std::vector<my_mesh::VertexHandle>  face_vhandles;

    int m = n - 1;

    //for on all the value, creating a cube
    for (int indice = 0; indice < (m*m*m); indice++)
    {

        int k = fmod(indice,m);              //z
        int j = (fmod(indice, m*m) - k) / m; //y
        int i = (indice - j*m - k) / (m*m);  //x

        int z = k;
        int y = j;
        int x = i;


        //vertex position x,y,z based on unit
        QList <Vec3f> cubeVert;

        //index of the cube vertex from the scalar i,j,k
        QList <int> cubo;

        cubo.append( n*n*i + n*j     +  k    ); //v0
        cubo.append( n*n*i + n*(j+1) +  k    ); //v1
        cubo.append( n*n*i + n*j     + (k+1) ); //v2
        cubo.append( n*n*i + n*(j+1) + (k+1) ); //v3

        cubeVert.append(Vec3f(x, y,   z  ));
        cubeVert.append(Vec3f(x, y+1, z  ));
        cubeVert.append(Vec3f(x, y,   z+1));
        cubeVert.append(Vec3f(x, y+1, z+1));


        cubo.append( n*n*(i+1) + n*j     +  k    ); //v4
        cubo.append( n*n*(i+1) + n*(j+1) +  k    ); //v5
        cubo.append( n*n*(i+1) + n*j     + (k+1) ); //v6
        cubo.append( n*n*(i+1) + n*(j+1) + (k+1) ); //v7

        cubeVert.append(Vec3f(x+1, y,   z  ));
        cubeVert.append(Vec3f(x+1, y+1, z  ));
        cubeVert.append(Vec3f(x+1, y,   z+1));
        cubeVert.append(Vec3f(x+1, y+1, z+1));

        int config = 0;

        //get the configuration of the current cube
        for(int ver = 0; ver < 8; ver++)
        {
            if(vec_num[cubo[ver]] < threshold_value )
            {

                config += pow(2, ver);
            }
        }

        //read the table from taula (look up table = lut)
        //defined in the header the object mccases

        std::vector<std::vector<int>> lut = mccases.operator ()(config);

        //analyze the given configuration and create the vertex
        for(int tr = 0; tr < int(lut.size()); tr++ )
        {

            //clear the face
            face_vhandles.clear();

            //build a new triangle
            for(int ver = 0; ver < int(lut[tr].size()); ver++)
            {
                my_mesh::Point new_point;

                //edge 0
                if( lut[tr][ver] == 0)
                {
                    Vec3f np = linear_interpolation(cubeVert[0], vec_num[cubo[0]], cubeVert[4], vec_num[cubo[4]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));
//                    std::cout << "indice :" << indice << " triangolo :" << tr << " vertice : " << ver << " edge " << 0 <<  " coords : " << np[0] <<  " " << np[1] << " " << np[2] << std::endl;

                }
                //edge 1
                else if( lut[tr][ver] == 1)
                {
                    Vec3f np = linear_interpolation(cubeVert[4], vec_num[cubo[4]], cubeVert[5], vec_num[cubo[5]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));
//                    std::cout << "indice :" << indice << " triangolo :" << tr << " vertice : " << ver << " edge " << 1 << " coords : " << np[0] <<  " " << np[1] << " " << np[2] << std::endl;


                }
                //edge 2
                else if( lut[tr][ver] == 2)
                {
                    Vec3f np = linear_interpolation(cubeVert[1], vec_num[cubo[1]], cubeVert[5], vec_num[cubo[5]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));
//                    std::cout << "indice :" << indice << " triangolo :" << tr << " vertice : " << ver << " edge " << 2  << " coords : " << np[0] <<  " " << np[1] << " " << np[2] << std::endl;

                }
                //edge 3
                else if( lut[tr][ver] == 3)
                {
                    Vec3f np = linear_interpolation(cubeVert[0], vec_num[cubo[0]], cubeVert[1], vec_num[cubo[1]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));
//                    std::cout << "indice :" << indice << " triangolo :" << tr << " vertice : " << ver << " edge " << 3  << " coords : " << np[0] <<  " " << np[1] << " " << np[2] << std::endl;

                }
                //edge 4
                else if( lut[tr][ver] == 4)
                {
                    Vec3f np = linear_interpolation(cubeVert[6], vec_num[cubo[6]], cubeVert[2], vec_num[cubo[2]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));

                }
                //edge 5
                else if( lut[tr][ver] == 5)
                {
                    Vec3f np = linear_interpolation(cubeVert[6], vec_num[cubo[6]], cubeVert[7], vec_num[cubo[7]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));

                }
                //edge 6
                else if( lut[tr][ver] == 6)
                {
                    Vec3f np = linear_interpolation(cubeVert[3], vec_num[cubo[3]], cubeVert[7], vec_num[cubo[7]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));

                }
                //edge 7
                else if( lut[tr][ver] == 7)
                {
                    Vec3f np = linear_interpolation(cubeVert[2], vec_num[cubo[2]], cubeVert[3], vec_num[cubo[3]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));

                }
                //edge 8
                else if( lut[tr][ver] == 8)
                {
                    Vec3f np = linear_interpolation(cubeVert[6], vec_num[cubo[6]], cubeVert[4], vec_num[cubo[4]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));

                }
                //edge 9
                else if( lut[tr][ver] == 9)
                {
                    Vec3f np = linear_interpolation(cubeVert[5], vec_num[cubo[5]], cubeVert[7], vec_num[cubo[7]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));

                }
                //edge 10
                else if( lut[tr][ver] == 10)
                {
                    Vec3f np = linear_interpolation(cubeVert[0], vec_num[cubo[0]], cubeVert[2], vec_num[cubo[2]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));

                }
                //edge 11
                else if( lut[tr][ver] == 11)
                {
                    Vec3f np = linear_interpolation(cubeVert[1], vec_num[cubo[1]], cubeVert[3], vec_num[cubo[3]]);
                    new_point = my_mesh::Point(np[0],np[1],np[2]);
                    vhandle[ver] = mesh.add_vertex(my_mesh::Point(np[0],np[1],np[2]));
                    face_vhandles.push_back(vhandle[ver]);
//                    face_vhandles.push_back(mesh.add_vertex(new_point));

                }
                else
                    std::cout << " c'e' un problema, siamo fuori dai casi accettabili :( " << std::endl;
            }

            count_faces ++;
            mesh.add_face(face_vhandles);

        }
    }

    std::cout << "numero di faccie " << count_faces <<std::endl;

    try
    {
      if ( !OpenMesh::IO::write_mesh(mesh, new_mesh_name.toStdString()) )
      {
        std::cerr << "Cannot write mesh to file" << std::endl;
        return 1;
      }
    }
    catch( std::exception& x )
    {
      std::cerr << x.what() << std::endl;
      return 1;
    }

    // we open the mesh we created
    open_mesh(new_mesh_name.toLocal8Bit(), _opt);

    //remove the temporary file from the folder
    if( remove( new_mesh_name.toLocal8Bit()) != 0 )
      std::cout << "not deleted "<<std::endl;
    else
      std::cout << "deleted "<<std::endl;


    return 0;


}

template <typename M>
OpenMesh::Vec3f MeshViewerWidgetT<M>::linear_interpolation(OpenMesh::Vec3f a,float va, OpenMesh::Vec3f b, float vb)
{
    OpenMesh::Vec3f solution;
    float v = threshold_value;
//    if(va == vb)
//    {
//        std::cout << " primo vettore : " << a[0] << " " << a[1] << " " << a[2] << " valore " << va << " primo vettore : " << b[0] << " " << b[1] << " " << b[2] << " valore " << vb << std::endl;
//        solution = a*( abs(v - va)/abs(vb - va)) + b*(abs(v - vb)/abs(vb -va));
//    }
//    else
      //  solution = a*( abs(v - va)/abs(vb - va)) + b*(abs(v - vb)/abs(vb -va));


//        float t = abs
    (v/(va - vb));

//        if( t < 0 || t > 1  )
//            std::cout << " something" << std::endl;

//        solution = a + t * (b-a);// * dotSign;
//        Vec3f vab = b - a;
//        Vec3f z =  solution - a;
//        float dot = vab[0]*z[0] + vab[1]*z[1] + vab[2]*z[2];
//        float dotSign = (dot < 0 ? -1 : 1);
//        t = 0.9;
//        solution = a + t * (b-a) * dotSign;

////        solution = b;
//      solution = a*( abs(v - va)/abs(vb - va)) + b*(abs(v - vb)/abs(vb -va));

      if(va > vb )
      {
          solution = b*(va - v)/(va - vb) + a*(v - vb)/(va - vb);
      }
      else{
          solution = a*(vb - v)/(vb - va) + b*(v - va)/(vb - va);
      }


        return solution;

//    solution = (a + b)/2;
//    solution = a + (b - a)*t;
//    return solution;


}
template <typename M>
bool MeshViewerWidgetT<M>::compute_oct_mesh( IO::Options _opt)
{
    my_mesh mesh;

    my_mesh::VertexHandle vhandle[6];
    std::vector<my_mesh::VertexHandle>  face_vhandles;

    float h = unit/2;
    std::cout<< "half unit" << h <<"unit" << unit <<  std::endl;

    for (int indice = 0; indice < n*n*n; indice++)
    {
        if(vec_num[indice] < threshold_value)
        {

            int z = fmod(indice,n); //K
            int y = (fmod(indice, n*n) - z)/n;//J
            int x = (indice - y*n-z)/(n*n);//I

            x = x*unit;
            y = y*unit;
            z = z*unit;

            // generate vertices
            vhandle[0] = mesh.add_vertex(my_mesh::Point( x + h, y,     z     ));
            vhandle[1] = mesh.add_vertex(my_mesh::Point( x,     y + h, z     ));
            vhandle[2] = mesh.add_vertex(my_mesh::Point( x - h, y,     z     ));
            vhandle[3] = mesh.add_vertex(my_mesh::Point( x,     y - h, z     ));
            vhandle[4] = mesh.add_vertex(my_mesh::Point( x,     y,     z + h ));
            vhandle[5] = mesh.add_vertex(my_mesh::Point( x,     y,     z - h ));

            // generate faces
            face_vhandles.clear();
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[4]);
            mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[4]);
            mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[4]);
            mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[4]);
            mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[5]);
            mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[5]);
            mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[5]);
            mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[5]);
            mesh.add_face(face_vhandles);

        }
    }

    try
    {
      if ( !OpenMesh::IO::write_mesh(mesh, new_mesh_name.toStdString()) )
      {
        std::cerr << "Cannot write mesh to file" << std::endl;
        return 1;
      }
    }
    catch( std::exception& x )
    {
      std::cerr << x.what() << std::endl;
      return 1;
    }

    // we open the mesh we created
    open_mesh(new_mesh_name.toLocal8Bit(), _opt);

    //remove the temporary file from the folder
    if( remove( new_mesh_name.toLocal8Bit()) != 0 )
      std::cout << "not deleted "<<std::endl;
    else
      std::cout << "deleted "<<std::endl;


    return 0;
}



template <typename M>
bool MeshViewerWidgetT<M>::compute_cube_mesh(IO::Options _opt)
{
    my_mesh mesh;

    my_mesh::VertexHandle vhandle[8];
    std::vector<my_mesh::VertexHandle>  face_vhandles;

    float h = unit/2;
    std::cout<< "half unit" << h <<"unit" << unit <<  std::endl;

    for (int indice = 0; indice < n*n*n; indice++)
    {
        if(vec_num[indice] < threshold_value)
        {

            int z = fmod(indice,n); //K
            int y = (fmod(indice, n*n) - z)/n;//J
            int x = (indice - y*n-z)/(n*n);//I

            x = x*unit;
            y = y*unit;
            z = z*unit;


            // generate vertices
            vhandle[0] = mesh.add_vertex(my_mesh::Point(-h + x, -h + y,  h + z));
            vhandle[1] = mesh.add_vertex(my_mesh::Point( h + x, -h + y,  h + z));
            vhandle[2] = mesh.add_vertex(my_mesh::Point( h + x,  h + y,  h + z));
            vhandle[3] = mesh.add_vertex(my_mesh::Point(-h + x,  h + y,  h + z));
            vhandle[4] = mesh.add_vertex(my_mesh::Point(-h + x, -h + y, -h + z));
            vhandle[5] = mesh.add_vertex(my_mesh::Point( h + x, -h + y, -h + z));
            vhandle[6] = mesh.add_vertex(my_mesh::Point( h + x,  h + y, -h + z));
            vhandle[7] = mesh.add_vertex(my_mesh::Point(-h + x,  h + y, -h + z));


            // generate faces
            face_vhandles.clear();
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[3]);
            mesh.add_face(face_vhandles);
            face_vhandles.clear();
            face_vhandles.push_back(vhandle[7]);
            face_vhandles.push_back(vhandle[6]);
            face_vhandles.push_back(vhandle[5]);
            face_vhandles.push_back(vhandle[4]);
            mesh.add_face(face_vhandles);
            face_vhandles.clear();
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[4]);
            face_vhandles.push_back(vhandle[5]);
            mesh.add_face(face_vhandles);
            face_vhandles.clear();
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[5]);
            face_vhandles.push_back(vhandle[6]);
            mesh.add_face(face_vhandles);
            face_vhandles.clear();
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[6]);
            face_vhandles.push_back(vhandle[7]);
            mesh.add_face(face_vhandles);
            face_vhandles.clear();
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[7]);
            face_vhandles.push_back(vhandle[4]);
            mesh.add_face(face_vhandles);

        }
    }

    try
    {
      if ( !OpenMesh::IO::write_mesh(mesh, new_mesh_name.toStdString()) )
      {
        std::cerr << "Cannot write mesh to file" << std::endl;
        return 1;
      }
    }
    catch( std::exception& x )
    {
      std::cerr << x.what() << std::endl;
      return 1;
    }

    // we open the mesh we created
    open_mesh(new_mesh_name.toLocal8Bit(), _opt);

    //remove the temporary file from the folder
    if( remove( new_mesh_name.toLocal8Bit()) != 0 )
      std::cout << "not deleted "<<std::endl;
    else
      std::cout << "deleted "<<std::endl;


    return 0;
}


template <typename M>
bool MeshViewerWidgetT<M>::create_ochtaedron(int n, float unit, QString name)
{
    IO::ImporterT<Mesh> importer(mesh_);
    VertexHandle vhandle[6];
    std::vector<VertexHandle>  face_vhandles;

    float h = unit/2;
    std::cout<< "half unit " << h <<" unit " << unit <<  std::endl;

    for (int indice = 0; indice < n*n*n; indice++)
    {
        if(vec_num[indice] < threshold_value)
        {
            int z = fmod(indice,n); //K
            int y = (fmod(indice, n*n) - z)/n;//J
            int x = (indice - y*n-z)/(n*n);//I

            x = x*unit;
            y = y*unit;
            z = z*unit;

            std::cout<< "position" << x << " " << y << " " << z << std::endl;

            // generate vertices
            vhandle[0] = importer.add_vertex(Vec3f( x + h, y,     z     ));
            vhandle[1] = importer.add_vertex(Vec3f( x,     y + h, z     ));
            vhandle[2] = importer.add_vertex(Vec3f( x - h, y,     z     ));
            vhandle[3] = importer.add_vertex(Vec3f( x,     y - h, z     ));
            vhandle[4] = importer.add_vertex(Vec3f( x,     y,     z + h ));
            vhandle[5] = importer.add_vertex(Vec3f( x,     y,     z - h ));

            // generate faces
            face_vhandles.clear();
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[4]);
            importer.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[4]);
            importer.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[4]);
            importer.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[4]);
            importer.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[5]);
            importer.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[1]);
            face_vhandles.push_back(vhandle[5]);
            importer.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[2]);
            face_vhandles.push_back(vhandle[5]);
            importer.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandle[0]);
            face_vhandles.push_back(vhandle[3]);
            face_vhandles.push_back(vhandle[5]);
            importer.add_face(face_vhandles);

        }
    }
    std::cout << "sono qui " << std::endl;
    mesh_.update_face_normals();
    mesh_.update_vertex_normals();
    mesh_.release_vertex_colors();
    mesh_.release_face_colors();
    // bounding box
    typename Mesh::ConstVertexIter vIt(mesh_.vertices_begin());
    typename Mesh::ConstVertexIter vEnd(mesh_.vertices_end());

//    typedef typename Mesh::Point Point;
    using OpenMesh::Vec3f;

    Vec3f bbMin, bbMax;

    bbMin = bbMax = OpenMesh::vector_cast<Vec3f>(mesh_.point(*vIt));

    for (size_t count=0; vIt!=vEnd; ++vIt, ++count)
    {
      bbMin.minimize( OpenMesh::vector_cast<Vec3f>(mesh_.point(*vIt)));
      bbMax.maximize( OpenMesh::vector_cast<Vec3f>(mesh_.point(*vIt)));
    }


    // set center and radius
    set_scene_pos( (bbMin+bbMax)*0.5, (bbMin-bbMax).norm()*0.5 );

    // for normal display
    normal_scale_ = (bbMax-bbMin).min()*0.05f;

    // info
    std::clog << mesh_.n_vertices() << " vertices, "
          << mesh_.n_edges()    << " edge, "
          << mesh_.n_faces()    << " faces\n";

    // base point for displaying face normals
    {
      OpenMesh::Utils::Timer t;
      t.start();
      mesh_.add_property( fp_normal_base_ );
      typename M::FaceIter f_it = mesh_.faces_begin();
      typename M::FaceVertexIter fv_it;
      for (;f_it != mesh_.faces_end(); ++f_it)
      {
        typename Mesh::Point v(0,0,0);
        for( fv_it=mesh_.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
          v += OpenMesh::vector_cast<typename Mesh::Normal>(mesh_.point(*fv_it));
        v *= 1.0f/3.0f;
        mesh_.property( fp_normal_base_, *f_it ) = v;
      }
      t.stop();
      std::clog << "Computed base point for displaying face normals ["
                << t.as_string() << "]" << std::endl;
    }

    //
    {
      std::clog << "Computing strips.." << std::flush;
      OpenMesh::Utils::Timer t;
      t.start();
      compute_strips();
      t.stop();
      std::clog << "done [" << strips_.n_strips()
        << " strips created in " << t.as_string() << "]\n";
    }

    #if defined(WIN32)
        updateGL();
    #endif


    // loading done
    return true;
}




template <typename M>
bool 
MeshViewerWidgetT<M>::open_mesh(const char* _filename, IO::Options _opt)
{
  // load mesh
  // calculate normals
  // set scene center and radius
  mesh_.request_face_normals();
  mesh_.request_face_colors();
  mesh_.request_vertex_normals();
  mesh_.request_vertex_colors();
  mesh_.request_vertex_texcoords2D();
  
  std::cout << "Loading from file '" << _filename << "'\n";
  if ( IO::read_mesh(mesh_, _filename, _opt ))
  {
    // store read option
    opt_ = _opt;
    
    // update face and vertex normals     
    if ( ! opt_.check( IO::Options::FaceNormal ) )
      mesh_.update_face_normals();
    else
      std::cout << "File provides face normals\n";
    
    if ( ! opt_.check( IO::Options::VertexNormal ) )
      mesh_.update_vertex_normals();
    else
      std::cout << "File provides vertex normals\n";


    // check for possible color information
    if ( opt_.check( IO::Options::VertexColor ) )
    {
      std::cout << "File provides vertex colors\n";
      add_draw_mode("Colored Vertices");
    }
    else
      mesh_.release_vertex_colors();


    if ( _opt.check( IO::Options::FaceColor ) )
    {
      std::cout << "File provides face colors\n";
      add_draw_mode("Solid Colored Faces");
      add_draw_mode("Smooth Colored Faces");
    }
    else
      mesh_.release_face_colors();

    if ( _opt.check( IO::Options::VertexTexCoord ) )
      std::cout << "File provides texture coordinates\n";


    // bounding box
    typename Mesh::ConstVertexIter vIt(mesh_.vertices_begin());
    typename Mesh::ConstVertexIter vEnd(mesh_.vertices_end());      
    
//    typedef typename Mesh::Point Point;
    using OpenMesh::Vec3f;
    
    Vec3f bbMin, bbMax;
    
    bbMin = bbMax = OpenMesh::vector_cast<Vec3f>(mesh_.point(*vIt));
    
    for (size_t count=0; vIt!=vEnd; ++vIt, ++count)
    {
      bbMin.minimize( OpenMesh::vector_cast<Vec3f>(mesh_.point(*vIt)));
      bbMax.maximize( OpenMesh::vector_cast<Vec3f>(mesh_.point(*vIt)));
    }
    
    
    // set center and radius
    set_scene_pos( (bbMin+bbMax)*0.5, (bbMin-bbMax).norm()*0.5 );
    
    // for normal display
    normal_scale_ = (bbMax-bbMin).min()*0.05f;
    
    // info
    std::clog << mesh_.n_vertices() << " vertices, "
	      << mesh_.n_edges()    << " edge, "
	      << mesh_.n_faces()    << " faces\n";
    
    // base point for displaying face normals
    {
      OpenMesh::Utils::Timer t;
      t.start();
      mesh_.add_property( fp_normal_base_ );
      typename M::FaceIter f_it = mesh_.faces_begin();
      typename M::FaceVertexIter fv_it;
      for (;f_it != mesh_.faces_end(); ++f_it)
      {
        typename Mesh::Point v(0,0,0);
        for( fv_it=mesh_.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
          v += OpenMesh::vector_cast<typename Mesh::Normal>(mesh_.point(*fv_it));
        v *= 1.0f/3.0f;
        mesh_.property( fp_normal_base_, *f_it ) = v;
      }
      t.stop();
      std::clog << "Computed base point for displaying face normals [" 
                << t.as_string() << "]" << std::endl;
    }

    //      
    {
      std::clog << "Computing strips.." << std::flush;
      OpenMesh::Utils::Timer t;
      t.start();
      compute_strips();
      t.stop();
      std::clog << "done [" << strips_.n_strips() 
		<< " strips created in " << t.as_string() << "]\n";
    }
    
    //    
#if defined(WIN32)
    updateGL();
#endif

    setWindowTitle(QFileInfo(_filename).fileName());

    // loading done
    return true;
  }
  return false;
}


//-----------------------------------------------------------------------------

template <typename M>
bool MeshViewerWidgetT<M>::open_texture( const char *_filename )
{
   QImage texsrc;
   QString fname = _filename;

   if (texsrc.load( fname ))
   {      
     return set_texture( texsrc );
   }
   return false;
}


//-----------------------------------------------------------------------------

template <typename M>
bool MeshViewerWidgetT<M>::set_texture( QImage& _texsrc )
{
  if ( !opt_.vertex_has_texcoord() )
    return false;
   
  {
    // adjust texture size: 2^k * 2^l
    int tex_w, w( _texsrc.width()  );
    int tex_h, h( _texsrc.height() );

    for (tex_w=1; tex_w <= w; tex_w <<= 1) {};
    for (tex_h=1; tex_h <= h; tex_h <<= 1) {};
    tex_w >>= 1;
    tex_h >>= 1;
    _texsrc = _texsrc.scaled( tex_w, tex_h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
  }

  QImage texture( QGLWidget::convertToGLFormat ( _texsrc ) );
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,   1);
  glPixelStorei(GL_PACK_ROW_LENGTH,    0);
  glPixelStorei(GL_PACK_SKIP_ROWS,     0);
  glPixelStorei(GL_PACK_SKIP_PIXELS,   0);
  glPixelStorei(GL_PACK_ALIGNMENT,     1);    
  
  if ( tex_id_ > 0 )
  {
    glDeleteTextures(1, &tex_id_);
  }
  glGenTextures(1, &tex_id_);
  glBindTexture(GL_TEXTURE_2D, tex_id_);
    
  // glTexGenfv( GL_S, GL_SPHERE_MAP, 0 );
  // glTexGenfv( GL_T, GL_SPHERE_MAP, 0 );
    
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);      
  
  glTexImage2D(GL_TEXTURE_2D,       // target
	       0,                   // level
	       GL_RGBA,             // internal format
	       texture.width(),     // width  (2^n)
	       texture.height(),    // height (2^m)
	       0,                   // border
	       GL_RGBA,             // format
	       GL_UNSIGNED_BYTE,    // type
	       texture.bits() );    // pointer to pixels

  std::cout << "Texture loaded\n";
  return true;
}


//-----------------------------------------------------------------------------

template <typename M>
void
MeshViewerWidgetT<M>::draw_openmesh(const std::string& _draw_mode)
{
  typename Mesh::ConstFaceIter    fIt(mesh_.faces_begin()), 
                                  fEnd(mesh_.faces_end());

  typename Mesh::ConstFaceVertexIter fvIt;

#if defined(OM_USE_OSG) && OM_USE_OSG
  if (_draw_mode == "OpenSG Indices") // --------------------------------------
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mesh_.points());

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, mesh_.vertex_normals());

    if ( tex_id_ && mesh_.has_vertex_texcoords2D() )
    {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, mesh_.texcoords2D());
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, tex_id_);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, tex_mode_);
    }

    glDrawElements(GL_TRIANGLES, 
		   mesh_.osg_indices()->size(), 
		   GL_UNSIGNED_INT, 
		   &mesh_.osg_indices()->getField()[0] );

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  else
#endif

  if (_draw_mode == "Wireframe") // -------------------------------------------
  {
     glBegin(GL_TRIANGLES);
     for (; fIt!=fEnd; ++fIt)
     {
        fvIt = mesh_.cfv_iter(*fIt);
        glVertex3fv( &mesh_.point(*fvIt)[0] );
        ++fvIt;
        glVertex3fv( &mesh_.point(*fvIt)[0] );
        ++fvIt;
        glVertex3fv( &mesh_.point(*fvIt)[0] );
     }
     glEnd();
  }
  
  else if (_draw_mode == "Solid Flat") // -------------------------------------
  {
    glBegin(GL_TRIANGLES);
    for (; fIt!=fEnd; ++fIt)
    {
      glNormal3fv( &mesh_.normal(*fIt)[0] );
      
      fvIt = mesh_.cfv_iter(*fIt);
      glVertex3fv( &mesh_.point(*fvIt)[0] );
      ++fvIt;
      glVertex3fv( &mesh_.point(*fvIt)[0] );
      ++fvIt;
      glVertex3fv( &mesh_.point(*fvIt)[0] );
    }
    glEnd();
    
  }


  else if (_draw_mode == "Solid Smooth") // -----------------------------------
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mesh_.points());

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, mesh_.vertex_normals());

    if ( tex_id_ && mesh_.has_vertex_texcoords2D() )
    {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, mesh_.texcoords2D());
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, tex_id_);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, tex_mode_);
    }

    glBegin(GL_TRIANGLES);
    for (; fIt!=fEnd; ++fIt)
    {
      fvIt = mesh_.cfv_iter(*fIt);
      glArrayElement(fvIt->idx());
      ++fvIt;
      glArrayElement(fvIt->idx());
      ++fvIt;
      glArrayElement(fvIt->idx());
    }
    glEnd();
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    if ( tex_id_ && mesh_.has_vertex_texcoords2D() )
    {
      glDisable(GL_TEXTURE_2D);
    }
  }  

  else if (_draw_mode == "Colored Vertices") // --------------------------------
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mesh_.points());

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, mesh_.vertex_normals());

    if ( mesh_.has_vertex_colors() )
    {
      glEnableClientState( GL_COLOR_ARRAY );
      glColorPointer(3, GL_UNSIGNED_BYTE, 0,mesh_.vertex_colors());
    }

    glBegin(GL_TRIANGLES);
    for (; fIt!=fEnd; ++fIt)
    {
      fvIt = mesh_.cfv_iter(*fIt);
      glArrayElement(fvIt->idx());
      ++fvIt;
      glArrayElement(fvIt->idx());
      ++fvIt;
      glArrayElement(fvIt->idx());
    }
    glEnd();
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
  }  


  else if (_draw_mode == "Solid Colored Faces") // -----------------------------
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mesh_.points());

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, mesh_.vertex_normals());

    glBegin(GL_TRIANGLES);
    for (; fIt!=fEnd; ++fIt)
    {
      glColor( *fIt );

      fvIt = mesh_.cfv_iter(*fIt);
      glArrayElement(fvIt->idx());
      ++fvIt;
      glArrayElement(fvIt->idx());
      ++fvIt;
      glArrayElement(fvIt->idx());
    }
    glEnd();
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }  


  else if (_draw_mode == "Smooth Colored Faces") // ---------------------------
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mesh_.points());

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, mesh_.vertex_normals());

    glBegin(GL_TRIANGLES);
    for (; fIt!=fEnd; ++fIt)
    {
      glMaterial( *fIt );

      fvIt = mesh_.cfv_iter(*fIt);
      glArrayElement(fvIt->idx());
      ++fvIt;
      glArrayElement(fvIt->idx());
      ++fvIt;
      glArrayElement(fvIt->idx());
    }
    glEnd();
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
  }  


  else if ( _draw_mode == "Strips'n VertexArrays" ) // ------------------------
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mesh_.points());

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, mesh_.vertex_normals());

    if ( tex_id_ && mesh_.has_vertex_texcoords2D() )
    {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, mesh_.texcoords2D());
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, tex_id_);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, tex_mode_);
    }

    typename MyStripifier::StripsIterator strip_it = strips_.begin();
    typename MyStripifier::StripsIterator strip_last = strips_.end();

    // Draw all strips
    for (; strip_it!=strip_last; ++strip_it)
    {
      glDrawElements(GL_TRIANGLE_STRIP, 
          static_cast<GLsizei>(strip_it->size()), GL_UNSIGNED_INT, &(*strip_it)[0] );
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }


  else if (_draw_mode == "Show Strips" && strips_.is_valid() ) // -------------
  {
    typename MyStripifier::StripsIterator strip_it = strips_.begin();
    typename MyStripifier::StripsIterator strip_last = strips_.end();

    float cmax  = 256.0f;
    int   range = 220;
    int   base  = (int)cmax-range;
    int   drcol  = 13;
    int   dgcol  = 31;
    int   dbcol  = 17;
    
    int rcol=0, gcol=dgcol, bcol=dbcol+dbcol;
         
    // Draw all strips
    for (; strip_it!=strip_last; ++strip_it)
    {
      typename MyStripifier::IndexIterator idx_it   = strip_it->begin();
      typename MyStripifier::IndexIterator idx_last = strip_it->end();
      
      rcol = (rcol+drcol) % range;
      gcol = (gcol+dgcol) % range;
      bcol = (bcol+dbcol) % range;
      
      glBegin(GL_TRIANGLE_STRIP);
      glColor3f((rcol+base)/cmax, (gcol+base)/cmax, (bcol+base)/cmax);
      for ( ;idx_it != idx_last; ++idx_it )
        glVertex3fv(&mesh_.point( OM_TYPENAME Mesh::VertexHandle(*idx_it))[0]);
      glEnd();
    }
    glColor3f(1.0, 1.0, 1.0);
  }


  else if( _draw_mode == "Points" ) // -----------------------------------------
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mesh_.points());

    if (mesh_.has_vertex_colors() && use_color_)
    {
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(3, GL_UNSIGNED_BYTE, 0, mesh_.vertex_colors());
    }

    glDrawArrays( GL_POINTS, 0, static_cast<GLsizei>(mesh_.n_vertices()) );
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
  }


}


//-----------------------------------------------------------------------------


template <typename M>
void 
MeshViewerWidgetT<M>::draw_scene(const std::string& _draw_mode)
{
  
  if ( ! mesh_.n_vertices() )
    return;
   
#if defined(OM_USE_OSG) && OM_USE_OSG
  else if ( _draw_mode == "OpenSG Indices")
  {     
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    draw_openmesh( _draw_mode );
  }     
  else
#endif
  if ( _draw_mode == "Points" )
  {
    glDisable(GL_LIGHTING);
    draw_openmesh(_draw_mode);
  }
  else if (_draw_mode == "Wireframe")
  {
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw_openmesh(_draw_mode);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  
  else if ( _draw_mode == "Hidden-Line" )
  {
    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor4f( 0.0f, 0.0f, 0.0f, 1.0f );
    glDepthRange(0.01, 1.0);
    draw_openmesh( "Wireframe" );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    glDepthRange( 0.0, 1.0 );
    draw_openmesh( "Wireframe" );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);          
  }
  
  else if (_draw_mode == "Solid Flat")
  {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    draw_openmesh(_draw_mode);
  }
  
  else if (_draw_mode == "Solid Smooth"        ||
	   _draw_mode == "Strips'n VertexArrays" )
  {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    draw_openmesh(_draw_mode);
  }
  
  else if (_draw_mode == "Show Strips")
  {
    glDisable(GL_LIGHTING);
    draw_openmesh(_draw_mode);
  }
  
  else if (_draw_mode == "Colored Vertices" )
  {
    glDisable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    draw_openmesh(_draw_mode);
  }

  else if (_draw_mode == "Solid Colored Faces")
  {
    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    draw_openmesh(_draw_mode);
    setDefaultMaterial();
  }
  
  else if (_draw_mode == "Smooth Colored Faces" )
  {
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    draw_openmesh(_draw_mode);
    setDefaultMaterial();
  }

  if (show_vnormals_)
  {
    typename Mesh::VertexIter vit;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.000f, 0.803f, 0.027f); // orange
    for(vit=mesh_.vertices_begin(); vit!=mesh_.vertices_end(); ++vit)
    {
      glVertex( *vit );
      glVertex( mesh_.point( *vit ) + normal_scale_*mesh_.normal( *vit ) );
    }
    glEnd();
  }

  if (show_fnormals_)
  {
    typename Mesh::FaceIter fit;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(0.705f, 0.976f, 0.270f); // greenish
    for(fit=mesh_.faces_begin(); fit!=mesh_.faces_end(); ++fit)
    {
      glVertex( mesh_.property(fp_normal_base_, *fit) );
      glVertex( mesh_.property(fp_normal_base_, *fit) +
                normal_scale_*mesh_.normal( *fit ) );
    }
    glEnd();
  }
}


//-----------------------------------------------------------------------------

template <typename M>
void 
MeshViewerWidgetT<M>::enable_strips() 
{ 
  if (!f_strips_)
  {
    f_strips_ = true;  
    add_draw_mode("Strips'n VertexArrays");
    add_draw_mode("Show Strips");
  }
}

//-----------------------------------------------------------------------------

template <typename M>
void 
MeshViewerWidgetT<M>::disable_strips() 
{ 
  if (f_strips_)
  {
    f_strips_ = false; 
    del_draw_mode("Show Strips");
    del_draw_mode("Strip'n VertexArrays");
  }
}


//-----------------------------------------------------------------------------

#define TEXMODE( Mode ) \
   tex_mode_ = Mode; std::cout << "Texture mode set to " << #Mode << std::endl

template <typename M>
void 
MeshViewerWidgetT<M>::keyPressEvent( QKeyEvent* _event)
{
  switch( _event->key() )
  {
    case Key_D:
      if ( mesh_.has_vertex_colors() && (current_draw_mode()=="Points") )
      {
        use_color_ = !use_color_;
        std::cout << "use color: " << (use_color_?"yes\n":"no\n");
        if (!use_color_)
          glColor3f(1.0f, 1.0f, 1.0f);
        updateGL();
      }
      break;

    case Key_N:
      if ( _event->modifiers() & ShiftModifier )
      {
        show_fnormals_ = !show_fnormals_;
        std::cout << "show face normals: " << (show_fnormals_?"yes\n":"no\n");
      }
      else
      {
        show_vnormals_ = !show_vnormals_;
        std::cout << "show vertex normals: " << (show_vnormals_?"yes\n":"no\n");
      }
      updateGL();
      break;

    case Key_I:
      std::cout << "\n# Vertices     : " << mesh_.n_vertices() << std::endl;
      std::cout << "# Edges        : " << mesh_.n_edges()    << std::endl;
      std::cout << "# Faces        : " << mesh_.n_faces()    << std::endl;
      std::cout << "binary  input  : " << opt_.check(opt_.Binary) << std::endl;
      std::cout << "swapped input  : " << opt_.check(opt_.Swap) << std::endl;
      std::cout << "vertex normal  : " 
                << opt_.check(opt_.VertexNormal) << std::endl;
      std::cout << "vertex texcoord: " 
                << opt_.check(opt_.VertexTexCoord) << std::endl;
      std::cout << "vertex color   : " 
                << opt_.check(opt_.VertexColor) << std::endl;
      std::cout << "face normal    : " 
                << opt_.check(opt_.FaceNormal) << std::endl;
      std::cout << "face color     : " 
                << opt_.check(opt_.FaceColor) << std::endl;
      this->QGLViewerWidget::keyPressEvent( _event );
      break;

    case Key_T:
      switch( tex_mode_ )
      {
        case GL_MODULATE: TEXMODE(GL_DECAL); break;
        case GL_DECAL:    TEXMODE(GL_BLEND); break;
        case GL_BLEND:    TEXMODE(GL_REPLACE); break;
        case GL_REPLACE:  TEXMODE(GL_MODULATE); break;
      }
      updateGL();
      break;

    default:
      this->QGLViewerWidget::keyPressEvent( _event );
  }
}

#undef TEXMODE

//=============================================================================



