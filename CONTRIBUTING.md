Contributing to OnlineOptimisation
==================================
- [Question or Problem?](#question)
- [Issues and Bugs](#issue)
- [Feature Requests](#feature)
- [Coding Guidelines](#coding)
- [Commit Guidelines](#commit)
- [Development Notes](#development)

<a name="question"></a> Got a Problem or Question?
--------------------------------------------------
If you have a question about how to use the OnlineOptimisation framework, please direct these to [StackOverflow](http://stackoverflow.com/questions/tagged/onlineoptimisation).

<a name="issue"></a> Found an Issue or Bug?
-------------------------------------------
If you found a bug in the source code or a mistake in any kind of documentation, please **let us know** by adding an issue to the [Github Issue Tracker](https://github.com/SebastianNiemann/OnlineOptimisation/issues).

You are welcomed to submit a pull request with your fix afterwards, if possible. **Please see our coding and commit guidelines below.**

<a name="feature"></a> Requesting a Feature?
--------------------------------------------
If you are missing some features within the OnlineOptimisation framework feel free to ask us about it by adding a new request to the [Github Issue Tracker](https://github.com/SebastianNiemann/OnlineOptimisation/issues) labelled `feature request`.

Note that a submitting a pull request providing the needed changes to introduced your requested feature usually speeds up the process. **Please see our coding and commit guidelines below.**

<a name="coding"></a> Coding Guidelines
---------------------------------------
- Everything must be **thoroughly tested**.
- All methods must be **documented**, regardless of the code visibility. And do not forget about inline comments for the complicated stuff.
- We usually follow the [Google Style Guides](https://code.google.com/p/google-styleguide/).

<a name="commit"></a> Commit Guidelines
---------------------------------------
Commit messages must be organised as followed:
```
[fix|feat|doc|test|devel]: <subject>

<body>

<signature>
```

### Prefix
The prefix is used to define the nature of the commit. Stick to one of the following tags:

- **fix:** Adding fixes
- **feat:** Adding a new feature
- **doc:** Changes to the documentation
- **test:** Adding new tests
- **devel:** Everything else

### Subject
Add a short description (usually no more than 15 words) about the content  of your commit.

### Body
Describe the details of your changes as specific as possible. Do not be afraid to add multiple lines. Please try to make your explanation as specific as possible and avoid vague descriptions.

Feel free to read the read the [Linux Kernel commit guidelines](http://git.kernel.org/cgit/git/git.git/tree/Documentation/SubmittingPatches?id=HEAD) on how to improve commit messages. 

### Signature
The signature only required in **external pull requests**. End the commit message of your pull request by adding the following DCO **and** signing your work.

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

<a name="development"></a> Development Notes
--------------------------------------------
We strive for an agile development process with monthly sprint meetings. New tasks are added any time via the [Github Issue Tracker](https://github.com/SebastianNiemann/OnlineOptimisation/issues) and assigned in the beginning of each sprint via [Waffle.io](https://waffle.io/sebastianniemann/onlineoptimisation). 

We make use of three kinds of branches:
- master
- `<user story branch>`
- `<hotfix branch>`

### Master
The main development branch, including the current state of the next release. Small changes may be pushed directly into this branch. The last task of each sprint is releasing the latests changes into `tag`.

**Lifetime:** Infinite

### User Story Branch
Each sprint may add several, individual `<user story branches>`. After completion of all tasks, each `<user story branch>` is merged into the `master` branch and removed afterwards.

**Lifetime:** Preferable one sprint

### Hotfix Branch
Based on the latests release in `tag` and removed after releasing the hotfix.

**Lifetime:** Very short, a few days at most
