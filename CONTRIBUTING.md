Contributing to OnlineOptimisation
==================================
- [Question or Problem?](#question)
- [Issues and Bugs](#issue)
- [Feature Requests](#feature)
- [Coding Guidelines](#coding)
- [Commit Guidelines](#commit)

<a name="question"></a> Got a Problem or Question?
--------------------------
If you have a question about how to use the OnlineOptimisation framework, please direct these to [StackOverflow](http://stackoverflow.com/questions/tagged/onlineoptimisation).

You may also want to check out our Getting Started.

<a name="issue"></a> Found an Issue or Bug?
----------------------
If you found a bug in the source code or a mistake in any kind of documentation, please **let us know** by adding an issue to the [Github Issue Tracker](https://github.com/SebastianNiemann/OnlineOptimisation/issues).

You are welcomed to submit a pull request with your fix afterwards, if possible. **Please see our coding and commit guidelines below.**

<a name="feature"></a> Requesting a Feature?
---------------------
If you are missing some features within the OnlineOptimisation framework feel free to ask us about it by adding a new request to the [Github Issue Tracker](https://github.com/SebastianNiemann/OnlineOptimisation/issues) labelled `feature request`.

Note that a submitting a pull request providing the needed changes to introduced your requested feature usually speeds up the process. **Please see our coding and commit guidelines below.**

<a name="coding"></a> Coding Guidelines
---------------------
- Everything must be **thoroughly tested** in compliance to our Master Test Plan.
- All methods must be **detailed documented**, regardless of their code visibility. This includes javadoc, as well as inline comments.
- We usually follow the [Google Style Guides](https://code.google.com/p/google-styleguide/).

<a name="commit"></a> Commit Guidelines
-----------------
Commit messages must be organised as followed:
```
<type>: <subject>

<body>

<signature>
```

#### Type
The `type` is use to sort in the nature of the commit. Stick to one of the following tags:

- **fix:** Adding fixes to the source code
- **feat:** Adding a new feature
- **doc:** Changes to the documentation
- **test:** Adding new tests
- **devel:** Changes concerning the development process (usually Travis CI)

#### Subject

Add a short description (usually no more than 15 words) about the content  of your commit.

#### Body

Describe the details of your changes as specific as possible. Do not be afraid to add multiple lines.

Please try to make your explanation as specific as possible and avoid vague descriptions.

If you submit a fix, include a list of fixed issues by adding:
```
Fixed:
#<issue number> <short issue description>
```

If you want to refer to a specific commit, add the SHA-1 hash together with the type and subject of the commit:
```
Commit <hash> (<type>: <subject>)
```

Feel free to read the read the [Linux Kernel commit guidelines](http://git.kernel.org/cgit/git/git.git/tree/Documentation/SubmittingPatches?id=HEAD) on how to improve commit messages. 

#### Signature
You are required to end each commit message by adding the following DCO and signing your work. Note that official contributors may be exempt from the former.

```
Developer's Certificate of Origin 1.1

By making a contribution to this project, I certify that:

(a) The contribution was created in whole or in part by me and I
    have the right to submit it under the open source license
    indicated in the file; or

(b) The contribution is based upon previous work that, to the best
    of my knowledge, is covered under an appropriate open source
    license and I have the right under that license to submit that
    work with modifications, whether created in whole or in part
    by me, under the same open source license (unless I am
    permitted to submit under a different license), as indicated
    in the file; or

(c) The contribution was provided directly to me by some other
    person who certified (a), (b) or (c) and I have not modified
    it.

(d) I understand and agree that this project and the contribution
    are public and that a record of the contribution (including all
    personal information I submit with it, including my sign-off) is
    maintained indefinitely and may be redistributed consistent with
    this project or the open source license(s) involved.
```

The signature should start with `Signed-off-by:` and include your real name (no pseudonyms or anonymous contributions) followed by some contact information.
```
Signed-off-by: Random J Developer <random@developer.example.org>
```

Finally, sign your commit via GPG by including `-S` to each git commit command after [creating your GPG key](https://www.gnupg.org/gph/en/manual.html#AEN26):
```
git commit -S
```
For official contributors committing directly via GitHub, GPG-signing is currently not enforced.
