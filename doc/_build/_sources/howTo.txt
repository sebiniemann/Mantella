HowTo Describe 
===============

Install Sqhinx, Doxygen and Breathe on your PC
-----------------------------------------------
.. code-block:: shell
       
 apt-get install python-sphinx
 apt-get install doxygen
 sudo pip install breathe
 sudo pip install sphinx_rtd_theme

For other Systems please read here:

	- `install_sphinx`_
	- `install_doxygen`_

.. _install_sphinx:	http://www.sphinx-doc.org/en/stable/install.html
.. _install_doxygen: http://www.stack.nl/~dimitri/doxygen/download.html#gitrepos
	

Generate documentation
----------------------

Go in path */mantella* and execute *./generateDoc.sh* .
The generated HTML files you will see in */doc/_bulid/* .

Describe in Code
------------------

Normally Comment
^^^^^^^^^^^^^^^^^
.. parsed-literal::
 /*! @brief Brief description.
  *    Brief description continued.
  *
  *  Detailed description starts here.
  */
   
Include image
^^^^^^^^^^^^^
.. parsed-literal::
  \\image html image.png
	
The image file must be in /doc_includes/image/

Include examples
^^^^^^^^^^^^^^^^
.. parsed-literal::
  \\include example.cpp

The example file must be in /doc_includes/example

Include HTML
^^^^^^^^^^^^
.. parsed-literal::
  \\verbatim embed:rst
    .. raw:: html
      <!DOCTYPE html>
      <html> 		
       	  Any HTML code
      </html>
  \\endverbatim
     
Include math
^^^^^^^^^^^^
.. parsed-literal::
  \\verbatim embed:rst
     .. math::
       Any Math
  \\endverbatim
	
Include code-block
^^^^^^^^^^^^^^^^^^
.. parsed-literal::
  \\verbatim embed:rst
    .. code-block:: [language]
    
		Any code
  
  \\endverbatim

	
Include inline math
^^^^^^^^^^^^^^^^^^	
.. parsed-literal::
     * \f{eqnarray*}{
     *      g &=& \frac{Gm_2}{r^2} \\
     *        &=& \frac{(6.673 \times 10^{-11}\,\mbox{m}^3\,\mbox{kg}^{-1}\,
     *            \mbox{s}^{-2})(5.9736 \times 10^{24}\,\mbox{kg})}{(6371.01\,\mbox{km})^2} \\
     *        &=& 9.82066032\,\mbox{m/s}^2
     * \f}
     */

   OR

     The distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is 
     \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.


Describe in rst
------------------

Added doxygen commends 
^^^^^^^^^^^^^^^^^^^^^^^
.. parsed-literal::
   \.. doxygenclass:: [Class] (mant::HillClimbing)  
      \:project: mantella
      \:protected-members: //If specified, the protected members of the class will be displayed.
      \:private-members: //If specified, the private members of the class will be displayed.
      \:undoc-members: //If specified, the undocumented members of the class will be displayed.
      \:outline: //This displays only the names of the class members and not their documentation.
      \:members: crack, isCracked //This displays the class documentation with only the members listed in the :members: option.1

To read on `Doc_Breathe`_
      
For more Info about `Doc_Sphinx`_

.. _Doc_Breathe: http://breathe.readthedocs.io/en/latest/directives.html
.. _Doc_Sphinx: http://www.sphinx-doc.org/en/stable/contents.html