
# Creation du site a partir du theme hacker

git remote add jekyll-theme-hacker https://github.com/pages-themes/hacker.git

git fetch jekyll-theme-hacker

git subtree add --prefix=docs/ --squash jekyll-theme-hacker v0.1.0 -m "[docs] Import du thème \"Hacker\" v0.1.0 pour le site Jekyll."

git remote remove jekyll-theme-hacker



# test local - install deps
cf. [https://github.com/pages-themes/hacker#previewing-the-theme-locally]

cd docs/
sudo apt-get install ruby-dev
sudo gem install ffi -v '1.9.18'
sudo ./script/bootstrap


# test local - exec

bundle exec jekyll serve
