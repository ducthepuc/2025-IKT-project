using System.Security.Cryptography;
using Microsoft.AspNetCore.Cryptography.KeyDerivation;

namespace Ecommerce.Core.Helpers
{
    public static class PasswordHelper
    {
        public static string HashPassword(string password)
        {
            byte[] salt = new byte[10]; //128 bit salt so even the same passwords will have a different hash
            using (var rng = RandomNumberGenerator.Create())
            {
                rng.GetBytes(salt);
            }

            //hash password with salt and PBKDF2(Password-Based Key Derivation Function 2)
            string hashedPassword = Convert.ToBase64String(KeyDerivation.Pbkdf2(
                password: password,
                salt: salt,
                prf: KeyDerivationPrf.HMACSHA256,
                iterationCount: 10000,
                numBytesRequested: 256 / 8));
            
            return $"{Convert.ToBase64String(salt)}${hashedPassword}";
        }

        public static bool VerifyPassword(string hashedPassword, string passwordToCheck)
        {
            var parts = hashedPassword.Split("$");
            var salt = Convert.FromBase64String(parts[0]);
            var storedHash = parts[1];

            string hashedInputPassword = Convert.ToBase64String(KeyDerivation.Pbkdf2(
                password: passwordToCheck,
                salt: salt,
                prf: KeyDerivationPrf.HMACSHA256,
                iterationCount: 10000,
                numBytesRequested: 256 / 8));
            
            return hashedInputPassword == hashedPassword;
        }
    }
}
